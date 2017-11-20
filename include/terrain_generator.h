//! @file       terrain_generator.h
//! @brief      Declares API for the Terrain_Generator type, which repesents a terrain generation
//!             algorithm
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

#pragma once

// CLarity Imports

// Standard Imports

// Third-Party Imports

namespace clarity 
{

//! @brief  An interface that represents a Terrain generaetion algorithm.
//!
//! @detail This API follows the strategy pattern. This is the interface for the strategy type
//! @todo   Use a more flexible pattern for passing parameters to underlying implementations.
class Terrain_Generator
{
public:

    //! @brief  Generate a Terrain map with the given size, scale, and roughness
    //!
    //! @param[in] rows         number of rows in the map
    //! @param[in] cols         numer of columns in the map
    //! @param[in] scale        scale of a cell, in meters per cell
    //! @param[in] roughness    the roughness of the generated terrain, in range [0, 1)
    virtual Terrain generate_terrain(const uint32_t rows, 
                                     const uint32_t cols, 
                                     const float scale, 
                                     const float roughness) = 0;

};

} 
