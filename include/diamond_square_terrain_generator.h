//! @file       diamond_square_terrain_generator.h
//! @brief      Declares an implementation of Terrain_Generator that utilizes the diamond-square
//!             algorithm to simulate realistic terrain
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

#pragma once

// CLarity Imports
#include "terrain.h"
#include "terrain_generator.h"

// Standard Imports

// Third-Party Imports

namespace clarity 
{

//! An implementation of Terrain_Generator based on the Diamond-Square algorithm for fractal
//! terrain generation.
//!
//! References:
//!     Hunter Loftis: http://www.playfuljs.com/realistic-terrain-in-130-lines/
//!     https://en.wikipedia.org/wiki/Diamond-square_algorithm
class Diamond_Square_Generator : public Terrain_Generator
{
public:
    
    Diamond_Square_Generator();


    ~Diamond_Square_Generator();


    Terrain generate_terrain(const uint32_t rows, 
                             const uint32_t cols, 
                             const float scale, 
                             const float roughness);

private:
    static constexpr float MAX_HEIGHT_M = 100;
};

}
