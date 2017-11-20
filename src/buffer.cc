//! @file       buffer.cc
//! @brief      Defines the Buffer type, which repesents a 2D area of memory in row-major order
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

Buffer::Buffer(const uint32_t rows, const uint32_t cols, const uint8_t depth)
    : m_rows(rows)
    , m_cols(cols)
    , m_depth(depth)
    , m_data(new float[m_rows * m_cols * m_depth])
{
    // Zero the array on initialization
    std::fill(m_data.get(), m_data.get() + (m_rows * m_cols * m_depth), 0.0);
}


Buffer::~Buffer()
{
    // No-op
}


Buffer::Buffer(const Buffer & other)
    : m_rows(other.m_rows)
    , m_cols(other.m_cols)
    , m_depth(other.m_depth)
    , m_data(other.m_data)
{
    // No-op 
}


Buffer & Buffer::operator=(const Buffer & other)
{
    if (&other == this) {
        return *this;
    }

    const auto size = other.size();
    m_rows = size.first;
    m_cols = size.second;
    m_depth = other.depth();
    m_data = other.m_data;

    return *this;
}


std::shared_ptr<float> Buffer::data()
{
    return m_data;
}


float & Buffer::at(const uint32_t row, const uint32_t col, uint8_t depth)
{
    if (row >= m_rows || col >= m_cols) {
        std::stringstream msg;
        msg << "(" << row << ", " << col << ") out of range for Buffer with size ";
        msg << "(" << m_rows << ", " << m_cols << ")";
        throw std::out_of_range(msg.str());
    }

    return *(m_data.get() + (row * m_cols + col * m_depth + depth));
}


std::pair<uint32_t, uint32_t> Buffer::size() const
{
    return std::make_pair(m_rows, m_cols);
}


uint8_t Buffer::depth() const
{
    return m_depth;
}

}
