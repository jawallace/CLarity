//! @file       diamond_square_terrain_generator.cc
//! @brief      Defines an implementation of Terrain_Generator that utilizes the diamond-square
//!             algorithm to simulate realistic terrain
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// CLarity Imports
#include "buffer.h"
#include "device_buffer.h"
#include "diamond_square_terrain_generator.h"
#include "terrain.h"
#include "terrain_generator.h"

// Standard Imports
#include <memory>
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


void _process_squares(const uint32_t rows, 
                      const uint32_t cols, 
                      const uint32_t size, 
                      const uint32_t half, 
                      std::mt19937 & gen, 
                      std::uniform_real_distribution<> & rng, 
                      Buffer & tbuffer)
{
    for (uint32_t r = half; r < rows; r += size) {
        for (uint32_t c = half; c < cols; c += size) {
            const bool lower_row_valid = r >= half;
            const bool upper_row_valid = (r + half) < rows;
            
            const bool lower_col_valid = c >= half;
            const bool upper_col_valid = (c + half) < cols;

            float sum = 0.0f;
            int valid_ct = 0;

            if (lower_row_valid && lower_col_valid) {
                sum += tbuffer.at(r - half, c - half);
                valid_ct++;
            }

            if (lower_row_valid && upper_col_valid) {
                sum += tbuffer.at(r - half, c + half);
                valid_ct++;
            }
            
            if (upper_row_valid && lower_col_valid) {
                sum += tbuffer.at(r + half, c - half);
                valid_ct++;
            }

            if (upper_row_valid && upper_col_valid) {
                sum += tbuffer.at(r + half, c + half);
                valid_ct++;
            }
            
            const float offset = rng(gen);
            tbuffer.at(r, c) = (sum / valid_ct) + offset;
        }
    }
}


void _process_diamonds(const uint32_t rows, 
                       const uint32_t cols, 
                       const uint32_t size, 
                       const uint32_t half, 
                       std::mt19937 & gen, 
                       std::uniform_real_distribution<> & rng, 
                       Buffer & tbuffer)
{
    for (uint32_t r = 0; r < rows; r += half) {
        uint32_t start_col = (r + half) % size;
        for (uint32_t c = start_col; c < cols; c += size) {
            const bool lower_row_valid = r >= half;
            const bool upper_row_valid = (r + half) < rows;
            
            const bool lower_col_valid = c >= half;
            const bool upper_col_valid = (c + half) < cols;

            float sum = 0.0f;
            int valid_ct = 0;
           
            if (lower_col_valid) {
                sum += tbuffer.at(r, c - half);
                valid_ct++;
            }
            
            if (upper_row_valid) {
                sum += tbuffer.at(r + half, c);
                valid_ct++;
            }
            
            if (upper_col_valid) {
                sum += tbuffer.at(r, c + half);
                valid_ct++;
            }
            
            if (lower_row_valid) {
                sum += tbuffer.at(r - half, c);
                valid_ct++;
            }

            const float offset = rng(gen);
            tbuffer.at(r, c) = (sum / valid_ct) + offset;
        }
    }
}


Terrain Diamond_Square_Generator::generate_terrain(std::shared_ptr<Buffer> buffer,
                                                   const float scale, 
                                                   const float roughness)
{
    Terrain terrain(buffer, scale);
    Buffer & tbuffer = terrain.data();

    uint32_t rows = std::get<0>(tbuffer.size());
    uint32_t cols = std::get<1>(tbuffer.size());

    // Initialize corners 
    tbuffer.at(0, 0) = Diamond_Square_Generator::MAX_HEIGHT_M / 2; 
    tbuffer.at(0, cols - 1) = Diamond_Square_Generator::MAX_HEIGHT_M / 2; 
    tbuffer.at(rows - 1, 0) = Diamond_Square_Generator::MAX_HEIGHT_M / 2; 
    tbuffer.at(rows - 1, cols - 1) = Diamond_Square_Generator::MAX_HEIGHT_M / 2;

    // Generate the tbuffer
    uint32_t size = (rows - 1);
    uint32_t half = size / 2;

    std::random_device rd;
    std::mt19937 gen(rd());

    while (half >= 1) {
        const float feature_scale = size * roughness;

        auto rng = std::uniform_real_distribution<>(-feature_scale, feature_scale);

        // Process squares
        _process_squares(rows, cols, size, half, gen, rng, tbuffer);

        // Process diamonds
        _process_diamonds(rows, cols, size, half, gen, rng, tbuffer);
        
        size = size / 2;
        half = size / 2;
    }

    return terrain;
}


Terrain Diamond_Square_Generator::generate_terrain(const uint32_t rows, 
                                                   const uint32_t cols, 
                                                   const float scale, 
                                                   const float roughness)
{
    if (rows != cols) {
        throw std::invalid_argument("Diamond-Square Generator requires that rows == cols");
    }

    std::shared_ptr<Buffer> buffer(new Buffer(rows, cols));
    return generate_terrain(buffer, scale, roughness);
}

}
