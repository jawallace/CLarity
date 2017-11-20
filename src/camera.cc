//! @file       camera.cc
//! @brief      Defines the Camera Type, which represents a camera looking at a scene
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// CLarity Imports
#include "camera.h"

// Standard Imports
#include <cmath>
#include <cstdint>
#include <tuple>
#include <utility>

// Third-Party Importss

namespace clarity
{

Camera::Camera(const float field_of_view_rad,
               const uint16_t rows,
               const uint16_t cols)
    : m_field_of_view_rad(field_of_view_rad)
    , m_focal_plane_size_pixels(rows, cols)
    , m_position(0., 0., 0.)
    , m_pointing_vector(1., 0., 0.)
{
    // No-op
}


Camera::~Camera()
{
    // No-op
}


Camera::Camera(const Camera & other)
    : m_field_of_view_rad(other.m_field_of_view_rad)
    , m_focal_plane_size_pixels(other.m_focal_plane_size_pixels)
    , m_position(other.m_position)
    , m_pointing_vector(other.m_pointing_vector)
{
    // No-op 
}


Camera & Camera::operator=(const Camera & other)
{
    if (this == &other) {
        return *this;
    }

    m_field_of_view_rad = other.m_field_of_view_rad;
    m_focal_plane_size_pixels = other.m_focal_plane_size_pixels;
    m_position = other.m_position;
    m_pointing_vector = other.m_pointing_vector;

    return *this;
}


// Reference: Francesco Callari https://stackoverflow.com/a/32530762
float Camera::focal_length() const
{
    return (m_focal_plane_size_pixels.second / 2.) / std::tan(m_field_of_view_rad / 2.);
}


float Camera::fov() const
{
    return m_field_of_view_rad;
}


void Camera::set_fov(const float fov_rad)
{
    m_field_of_view_rad = fov_rad;
}


const Camera::Size & Camera::focal_plane_dimensions() const
{
    return m_focal_plane_size_pixels;
}


const Camera::Position & Camera::position() const
{
    return m_position;
}


void Camera::set_position(const Camera::Position & pos)
{
    m_position = pos;
}


const Camera::Pointing_Vector & Camera::pointing_vector() const
{
    return m_pointing_vector;
}


void Camera::set_pointing_vector(const Camera::Pointing_Vector & pv)
{
    m_pointing_vector = pv;
}


}
