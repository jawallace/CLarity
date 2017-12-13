//! @file       cl_range_calculator.h
//! @brief      Declares an implementation of Range_Calculator that uses the CPU
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

#pragma once

// CLarity Imports
#include "buffer.h"
#include "camera.h"
#include "range_calculator.h"
#include "terrain.h"

// Standard Imports
#include <memory>
#include <vector>

// Third-Party Imports


namespace clarity
{

//! @brief  A (naive) Implementation of Range_Calculator for the CPU
class CPU_Range_Calculator : public Range_Calculator
{
public:
    //! @brief  Default constructor.
    CPU_Range_Calculator();


    //! @brief  Destructor
    ~CPU_Range_Calculator();


    //! @brief  Deleted copy constructor
    CPU_Range_Calculator(const CPU_Range_Calculator & other) = delete;


    //! @brief  Deleted assignment operator
    CPU_Range_Calculator & operator=(const CPU_Range_Calculator & other) = delete;


    //! @brief  See Range_Calculator::Calculate
    void Calculate(const Camera & cam, const Terrain & t, Buffer & rng);


    //! @brief  See Range_Calculator::Convert_Pixel_To_Camera_Coordinates
    void Convert_Pixel_To_Camera_Coordinates(const Camera & cam, Buffer & cam_coords);


    //! @brief  See Range_Calculator::Convert_Camera_To_World_Coordinates
    void Convert_Camera_To_World_Coordinates(const Camera & cam, 
                                             const Buffer & cam_coords, 
                                             Buffer & world_coords);


    //! @brief  See Range_Calculator::Compute_Range
    void Compute_Range(const Camera & cam, 
                       const Terrain & t, 
                       const Buffer & world_coords, 
                       Buffer & rng);

};

}
