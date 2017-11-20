//! @file       diamond_square_terrain_generator.cc
//! @brief      Defines an implementation of Terrain_Generator that utilizes the diamond-square
//!             algorithm to simulate realistic terrain
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// CLarity Imports
#include "diamond_square_terrain_generator.h"
#include "terrain.h"
#include "terrain_generator.h"

// Standard Imports
#include <random>
#include <stdexcept>

// Third-Party Imports

namespace clarity 
{

Diamond_Square_Generator::Diamond_Square_Generator()
    : Terrain_Generator()
{
    // No-op, no state needed
}


Diamond_Square_Generator::~Diamond_Square_Generator()
{
    // No-op, no resources allocated
}


Terrain Diamond_Square_Generator::generate_terrain(const uint32_t rows, 
                                                   const uint32_t cols, 
                                                   const float scale, 
                                                   const float roughness)
{
    if (rows != cols) {
        throw std::invalid_argument("Diamond-Square Generator requires that rows == cols");
    }

    Terrain terrain(rows, cols, scale);
    Buffer & tbuffer = terrain.data();

    // Initialize corners 
    tbuffer.at(0, 0) = Diamond_Square_Generator::MAX_HEIGHT_M / 2; 
    tbuffer.at(0, cols - 1) = Diamond_Square_Generator::MAX_HEIGHT_M / 2; 
    tbuffer.at(rows - 1, 0) = Diamond_Square_Generator::MAX_HEIGHT_M / 2; 
    tbuffer.at(rows - 1, cols - 1) = Diamond_Square_Generator::MAX_HEIGHT_M / 2;

    // Generate the tbuffer
    uint32_t size = rows;
    uint32_t half = rows / 2;

    std::random_device rd;
    std::mt19937 gen(rd());

    while (half >= 1) {
        const float feature_scale = size * roughness;

        auto rng = std::uniform_real_distribution<>(-feature_scale, feature_scale);

        // Process squares
        for (uint32_t r = half; r < rows; r += size) {
            for (uint32_t c = half; c < cols; c += size) {
                const float offset = rng(gen);
                
                const float sum = tbuffer.at(r - half, c - half) + tbuffer.at(r - half, c + half) +
                                  tbuffer.at(r + half, c - half) + tbuffer.at(r + half, c + half);

                tbuffer.at(r, c) = (sum / 4) + offset;
            }
        }
   
        // Process diamonds
        for (uint32_t r = 0; r < rows; r += half) {
            for (uint32_t c = 0; c < cols; c += half) {
                const float offset = rng(gen);
                
                const float sum = tbuffer.at(r, c - size) + tbuffer.at(r + size, c) + 
                                  tbuffer.at(r, c + size) + tbuffer.at(r - size, c);

                tbuffer.at(r, c) = (sum / 4) + offset;
            }
        }
        
        size = size / 2;
        half = size / 2;
    }

    return terrain;
}

}
