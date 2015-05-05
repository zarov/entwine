/******************************************************************************
* Copyright (c) 2016, Connor Manning (connor@hobu.co)
*
* Entwine -- Point cloud indexing
*
* Entwine is available under the terms of the LGPL2 license. See COPYING
* for specific license text and more information.
*
******************************************************************************/

#include <entwine/compression/util.hpp>

#include <pdal/Compression.hpp>
#include <pdal/PointLayout.hpp>

#include <entwine/compression/stream.hpp>
#include <entwine/types/schema.hpp>

namespace entwine
{

std::unique_ptr<std::vector<char>> Compression::compress(
        const std::vector<char>& data,
        const Schema& schema)
{
    return compress(data.data(), data.size(), schema);
}

std::unique_ptr<std::vector<char>> Compression::compress(
        const char* data,
        const std::size_t size,
        const Schema& schema)
{
    CompressionStream compressionStream;
    pdal::LazPerfCompressor<CompressionStream> compressor(
            compressionStream,
            schema.pdalLayout().dimTypes());

    compressor.compress(data, size);
    compressor.done();

    std::unique_ptr<std::vector<char>> compressed(
            new std::vector<char>(compressionStream.data().size()));

    std::memcpy(
            compressed->data(),
            compressionStream.data().data(),
            compressed->size());

    return compressed;
}

std::unique_ptr<std::vector<char>> Compression::decompress(
        const std::vector<char>& data,
        const Schema& schema,
        const std::size_t decompressedSize)
{
    CompressionStream compressionStream(data);
    pdal::LazPerfDecompressor<CompressionStream> decompressor(
            compressionStream,
            schema.pdalLayout().dimTypes());

    std::unique_ptr<std::vector<char>> decompressed(
            new std::vector<char>(decompressedSize));

    decompressor.decompress(decompressed->data(), decompressed->size());

    return decompressed;
}

void Compression::pushSize(std::vector<char>& data, uint64_t size)
{
    data.insert(data.end(), &size, &size + sizeof(uint64_t));
}

uint64_t Compression::popSize(std::vector<char>& data)
{
    uint64_t size(0);
    const std::size_t popSize(sizeof(uint64_t));

    if (data.size() < popSize)
    {
        throw std::runtime_error("Invalid uncompressed size");
    }

    std::memcpy(&size, data.data() + data.size() - popSize, popSize);
    data.resize(data.size() - popSize);

    return size;
}

} // namespace entwine

