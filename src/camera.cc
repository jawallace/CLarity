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
    , m_pitch_rad(0)
    , m_yaw_rad(0)
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
    , m_pitch_rad(other.m_pitch_rad)
    , m_yaw_rad(other.m_yaw_rad)
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
    m_yaw_rad = other.m_yaw_rad;
    m_pitch_rad = other.m_pitch_rad;

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


float Camera::yaw() const
{
    return m_yaw_rad;
}


void Camera::set_yaw(const float yaw)
{
    m_yaw_rad = yaw;
}


float Camera::pitch() const
{
    return m_pitch_rad;
}


void Camera::set_pitch(const float pitch)
{
    m_pitch_rad = pitch;
}


void Camera::get_rotation_matrix(std::shared_ptr<float> rot_buffer) const 
{
    //const float r = 0.0; 
    const float p = m_pitch_rad; 
    const float y = m_yaw_rad; 

    float * rot = rot_buffer.get();

    rot[0] = std::cos(p) * std::cos(y); 
    rot[1] = - std::cos(p) * std::sin(y);
    rot[2] = std::sin(p);
    rot[3] = 0.0;

    rot[4] = std::sin(y);
    rot[5] = std::cos(y);
    rot[6] = 0.0;
    rot[7] = 0.0;

    rot[8] = - std::cos(y) * std::sin(p);
    rot[9] = std::sin(p) * std::sin(y);
    rot[10] = std::cos(p);
    rot[11] = 0.0;
}


}
