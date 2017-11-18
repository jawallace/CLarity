//! @file       terrain.h
//! @brief      Declares the Terrain type, which repesents an area that can be rendered
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

//! @brief a 2-D heightmap of terrain data
class Terrain
{
public: 

    //! @brief Constructor for the Terrain type
    //! 
    //! @param[in] rows                 number of rows in the terrain map
    //! @param[in] cols                 number of cols in the terrain map
    //! @param[in] scale_m_per_cell     scale of a cell, in meters per cell/pixel
    Terrain(const uint32_t rows, const uint32_t cols, const double scale_m_per_cell);


    //! @brief Destructor for the Terrain type
    ~Terrain();


    //! @brief      Copy constructor
    //! @detail     The underlying data is not copied - just referenced counted.
    Terrain(const Terrain & other);


    //! @brief      Assignment operator
    //! @detail     The underlying data is not copied - just referenced counted.
    Terrain & operator=(const Terrain & other);


    //! @brief      Access the Terrain data
    //! @detail     Get a handle to the underling buffer
    std::shared_ptr<double> data();


    //! @brief      Get a handle to the element at the given row and column
    double & at(const uint32_t row, const uint32_t col);


    //! @brief      Get the size of the terrain map, in cells
    std::pair<uint32_t, uint32_t> size() const;


    //! @brief      Get the scale of each Terrain map cell
    double scale() const;

private:
    //! The number of rows in the terrain map
    uint32_t m_rows;

    //! The number of columns in the terrain map
    uint32_t m_cols;
    
    //! The scale of each cell, in meters per cell
    uint32_t m_scale_m_per_cell;
    
    //! The scale of each cell, in meters per cell
    std::shared_ptr<double> m_data;
};

}
