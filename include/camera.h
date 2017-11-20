//! @file       camera.h
//! @brief      Declares the Camera type, which repesents a camera in a scene.
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

#pragma once

// CLarity Imports

// Standard Imports
#include <cstdint>
#include <tuple>
#include <utility>

// Third-Party Importss

namespace clarity 
{

//! @brief  A model of a Camera in a scene
//!
//! @detail The intrinsic parameters that can be modeled are field of view and focal plane 
//!         dimensions. Subclasses can override the pixel-to-angle calculation to model complex
//!         lenses with distortions. All intrinsic parameters can be modified after creation with
//!         the exception of image size.
//!
//!         Extrinsic parameters are world position and pointing vector. All extrinsic parameters
//!         can be modified after creation.
class Camera
{

public:

    //! @brief  Defintion of a size type for convenience
    typedef std::pair<uint16_t, uint16_t> Size;


    //! @brief  Defintion of a position type for convenience
    typedef std::tuple<float, float, float> Position;


    //! @brief  Defintion of a poiting vector type for convenience
    typedef std::tuple<float, float, float> Pointing_Vector;


    //! @brief Constructor for a model of a camera
    //! 
    //! @param[in] field_of_view_rad    The field of view of the camera, in radians
    //! @param[in] rows                 The size of the vertical focal plane dimension, in pixels
    //! @param[in] cols                 The size of the horizontal focal plane dimension, in pixels
    Camera(const float field_of_view_rad,
           const uint16_t rows, 
           const uint16_t cols);


    //! @brief  Destructor for the Camera type
    virtual ~Camera();


    //! @brief  Copy Constructor
    Camera(const Camera & other);


    //! @brief  Assignment operator
    Camera & operator=(const Camera & other);


    //! @brief  Retrieve the focal length of the Camera, in pixels based on intrinsic parameters
    float focal_length() const;


    //! @brief  Retreieve the field of view of the sensor in radians
    float fov() const;


    //! @brief Set the field of view of the Camera
    //!
    //! @param[in]  fov_rad     The new field of view of the Camera, in radians
    void set_fov(const float fov_rad);


    //! @brief  Retrieve the focal plane dimensions of the camera, in pixels
    const Size & focal_plane_dimensions() const;


    //! @brief  Get the current position of the Camera in world coordinates
    const Position & position() const;

    
    //! @brief Set the current position of the Camera
    //!
    //! @param[in]  p   The new position of the Camera
    void set_position(const Position & p);


    //! @brief Get the current pointing vector of the Camera, in world coordinates
    const Pointing_Vector & pointing_vector() const;


    //! @brief Set the current pointing vector of the Camera
    //!
    //! @param[in]  p   The new pointing vector of the Camera
    void set_pointing_vector(const Pointing_Vector & p);

private:

    //! The field of view of the Camera, in radians
    float m_field_of_view_rad;

    //! The dimensions of the focal plane, in pixels
    Size m_focal_plane_size_pixels;

    //! The position of the Camera, in world coordinates
    Position m_position;

    //! The pointing vector of the Camera, in world coordinates
    Pointing_Vector m_pointing_vector;
};

}
