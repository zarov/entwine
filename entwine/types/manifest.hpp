/******************************************************************************
* Copyright (c) 2019, Connor Manning (connor@hobu.co)
*
* Entwine -- Point cloud indexing
*
* Entwine is available under the terms of the LGPL2 license. See COPYING
* for specific license text and more information.
*
******************************************************************************/

#pragma once

#include <cstdint>
#include <string>

#include <entwine/types/bounds.hpp>
#include <entwine/types/source.hpp>

namespace entwine
{
namespace manifest
{

struct Item
{
    Item(const source::Source& s)
        : path(s.path)
        , points(s.info.points)
        , bounds(s.info.bounds)
        , info(s.info)
    { }

    /*
    Item(std::string path, uint64_t points, Bounds bounds)
        : path(path)
        , points(points)
        , bounds(bounds)
    { }
    */

    std::string path;
    uint64_t points = 0;
    Bounds bounds;

    bool inserted = false;
    std::string error;

    optional<source::Info> info;
};

using ItemList = std::vector<Item>;

ItemList create(const source::List& sources)
{
    return ItemList(sources.begin(), sources.end());
}

} // namespace manifest
} // namespace entwine
