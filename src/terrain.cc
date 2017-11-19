//! @file       terrain.cc
//! @brief      Defines the Terrain type, which repesents an area that can be rendered
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// Clarity Imports
#include "buffer.h"
#include "terrain.h"

// Standard Imports
#include <algorithm>
#include <cstdint>
#include <exception>
#include <memory>
#include <sstream>

// Third-Party Imports

namespace clarity
{

Terrain::Terrain(const uint32_t rows, const uint32_t cols, const double scale_m_per_cell)
    : m_buffer(new Buffer(rows, cols))
    , m_scale_m_per_cell(scale_m_per_cell)
{
    // No-op
}


Terrain::Terrain(std::shared_ptr<Buffer> buffer, const double scale_m_per_cell)
    : m_buffer(buffer)
    , m_scale_m_per_cell(scale_m_per_cell)
{
    // No-op
}


Terrain::~Terrain()
{
    // No-op
}


Terrain::Terrain(const Terrain & other)
    : m_buffer(other.m_buffer)
    , m_scale_m_per_cell(other.scale())
{
    // No-op 
}


Terrain & Terrain::operator=(const Terrain & other)
{
    if (&other == this) {
        return *this;
    }

    m_buffer = other.m_buffer;
    m_scale_m_per_cell = other.scale();

    return *this;
}


Buffer & Terrain::data()
{
    return *m_buffer;
}


double Terrain::scale() const
{
    return m_scale_m_per_cell;
}

}
