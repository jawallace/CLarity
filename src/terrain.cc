//! @file       terrain.cc
//! @brief      Defines the Terrain type, which repesents an area that can be rendered
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// Clarity Imports
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
    : m_rows(rows)
    , m_cols(cols)
    , m_scale_m_per_cell(scale_m_per_cell)
    , m_data(new double[m_rows * m_cols])
{
    // Zero the array on initialization
    std::fill(m_data.get(), m_data.get() + (m_rows * m_cols), 0.0);
}


Terrain::~Terrain()
{
    // No-op
}


Terrain::Terrain(const Terrain & other)
    : m_rows(other.m_rows)
    , m_cols(other.m_cols)
    , m_scale_m_per_cell(other.scale())
    , m_data(other.m_data)
{
    // No-op 
}


Terrain & Terrain::operator=(const Terrain & other)
{
    if (&other == this) {
        return *this;
    }

    const auto size = other.size();
    m_rows = size.first;
    m_cols = size.second;
    m_scale_m_per_cell = other.m_scale_m_per_cell;
    m_data = other.m_data;

    return *this;
}


std::shared_ptr<double> Terrain::data()
{
    return m_data;
}


double & Terrain::at(const uint32_t row, const uint32_t col)
{
    if (row >= m_rows || col >= m_cols) {
        std::stringstream msg;
        msg << "(" << row << ", " << col << ") out of range for Terrain with size ";
        msg << "(" << m_rows << ", " << m_cols << ")";
        throw std::out_of_range(msg.str());
    }

    return *(m_data.get() + (row * m_cols + col));
}


std::pair<uint32_t, uint32_t> Terrain::size() const
{
    return std::make_pair(m_rows, m_cols);
}


double Terrain::scale() const
{
    return m_scale_m_per_cell;
}

}
