//! @file       range_calculator.h
//! @brief      Declares the main range-calculation API
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

#pragma once

// CLarity Imports
#include "buffer.h"
#include "camera.h"
#include "terrain.h"

// Standard Imports

// Third-Party Imports


namespace clarity
{


//! @brief  The interface for the CLarity Range Calculation.
//!
//! @detail This is a pure-virtual class that defines the interface for the Range_Calcuator.
//!         Concrete Implementations will provide the actual implementations (which are possibly
//!         GPU-accelereated)
class Range_Calculator
{

    //! @brief      Compute the range image from the given Camera and Terrain
    //!
    //! @detail     This is a convenience function that, at it's simplest, calls the other methods
    //!             in the Range_Caclulator API in the correct order. Concrete implementations are 
    //!             free to use a streamlined or otherwise optimized computation here.
    //!
    //! @param[in]  cam     the Camera in the scene
    //! @param[in]  t       the Terrain in the scene
    //! @param[out] rng     a Buffer into which the range image will be placed. 
    virtual void Calculate(const Camera & cam, const Terrain & t, Buffer & rng) = 0;


    //! @brief      Compute the Camera coordinates of each pixel in the Camera.
    //!
    //! @detail     This function uses the instrisic paramaters of the Camera to compute the
    //!             pixel-to-angle mapping of each pixel. The result is a unit vector specifying
    //!             the pointing vector of the pixel relative to Camera boresight.
    //!
    //! @param[in]  cam         The Camera in the secne
    //! @param[out] cam_coords  a Buffer into which the Camera coordinates will be placed. 
    virtual void Convert_Pixel_To_Camera_Coordinates(const Camera & cam, 
                                                     Buffer & cam_coords) = 0;


    //! @brief      Compute the World coordinate of each pixel in the Camera.
    //!
    //! @detail     This function uses the Camera coordinates and the extrinisic Camera
    //!             parameters to determine the location in the World at which the pixel is
    //!             pointing. The result is a unit vector specifying the pointing vector of
    //!             the pixel relative to the World's origin.
    //! 
    //! @param[in]  cam             the Camera in the scene
    //! @param[in]  cam_coords      the camera coordinates of each pixel in the Camera
    //! @param[out] world_coords    a Buffer into which the Camera coordinates will be placed. 
    virtual void Convert_Camera_To_World_Coordinates(const Camera & cam, 
                                                     const Buffer & cam_coords, 
                                                     Buffer & world_coords) = 0;

    //! @brief      Compute the range image for the given Terrain and pointing vector buffer.
    //!  
    //! @param[in]  cam             the Camera in the scene
    //! @param[in]  t               The Terrain to map
    //! @param[in]  world_coords    the poiting vector of each pixel, in world coordinates
    //! @param[out] rng             a Buffer into which the range image will be placed.
    virtual void Compute_Range(const Camera & cam,
                               const Terrain & t, 
                               const Buffer & world_coords, 
                               Buffer & rng) = 0;

};


}

