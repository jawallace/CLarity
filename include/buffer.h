//! @file       buffer.h
//! @brief      Declares the Buffer type, which repesents 2D area of memory in row-major order.
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

#pragma once

// CLarity Imports

// Standard Imports
#include <cstdint>
#include <memory>
#include <utility>

// Third-Party Importss

namespace clarity 
{

//! @brief a 2-D area of memory in row-major order
//!
//! @detail This type supports efficient copies and assignments
class Buffer
{
public: 

    //! @brief Constructor for the Buffer type
    //! 
    //! @param[in] rows                 number of rows in the buffer
    //! @param[in] cols                 number of cols in the buffer
    //! @param[in] depth                the number of values at each point
    Buffer(const uint32_t rows, const uint32_t cols, const uint8_t depth = 1);


    //! @brief Destructor for the Buffer type
    virtual ~Buffer();


    //! @brief      Copy constructor
    //! @detail     The underlying data is not copied - just referenced counted.
    Buffer(const Buffer & other);


    //! @brief      Assignment operator
    //! @detail     The underlying data is not copied - just referenced counted.
    Buffer & operator=(const Buffer & other);


    //! @brief      Access the Buffer data
    //! @detail     Get a handle to the underling buffer
    std::shared_ptr<float> data();


    //! @brief      Get a handle to the element at the given row and column
    float & at(const uint32_t row, const uint32_t col, const uint8_t depth = 0);
   

    //! @brief      Get a handle to the element at the given row and column
    const float & at(const uint32_t row, const uint32_t col, const uint8_t depth = 0) const;


    //! @brief      Get the size of the Buffer, in cells
    std::pair<uint32_t, uint32_t> size() const;


    //! @brief      Get the depth of the Buffer
    uint8_t depth() const;

protected:
    //! The number of rows in the terrain map
    uint32_t m_rows;

    //! The number of columns in the terrain map
    uint32_t m_cols;
  
    // The number of values at each pixel.
    uint8_t m_depth;

    //! The scale of each cell, in meters per cell
    std::shared_ptr<float> m_data;
};

}
