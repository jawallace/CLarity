//! @file       cl_range_calculator.cc
//! @brief      Defines an implementation of Range_Calculator that uses the CPU
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// CLarity Imports
#include "buffer.h"
#include "camera.h"
#include "cpu_range_calculator.h"
#include "range_calculator.h"
#include "terrain.h"

// Standard Imports
#include <cmath>
#include <iostream>

// Third-Party Imports


namespace clarity
{


CPU_Range_Calculator::CPU_Range_Calculator()
{
   // No-op
}


CPU_Range_Calculator::~CPU_Range_Calculator()
{
    // No-op
}


float _dot(const float * a, const float * b)
{
    return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}


float _length(const float * a)
{
    return std::sqrt(std::pow(a[0], 2) + std::pow(a[1], 2) + std::pow(a[2], 2));
}


void CPU_Range_Calculator::Calculate(const Camera & cam, const Terrain & t, Buffer & rng)
{
    const auto sz = cam.focal_plane_dimensions();
    const auto num_rows = std::get<0>(sz);
    const auto num_cols = std::get<1>(sz);

    Buffer cam_coords(num_rows, num_cols, 4);
    Convert_Pixel_To_Camera_Coordinates(cam, cam_coords);

    Buffer world_coords(num_rows, num_cols, 4);
    Convert_Camera_To_World_Coordinates(cam, cam_coords, world_coords);

    Compute_Range(cam, t, world_coords, rng);
}


void CPU_Range_Calculator::Convert_Pixel_To_Camera_Coordinates(const Camera & cam, 
                                                               Buffer & cam_coords)
{
    const auto focal_length_pix = cam.focal_length();
    const auto sz = cam.focal_plane_dimensions();

    const auto num_rows = std::get<0>(sz);
    const auto num_cols = std::get<1>(sz);
    for (auto r = 0; r < num_rows; r++) {
        for (auto c = 0; c < num_cols; c++) {
            float pix[3] = { static_cast<float>(r - (num_rows / 2.0f)), 
                             static_cast<float>(c - (num_cols / 2.0f)), 
                             focal_length_pix };
            float center[3] = { 0.0, 0.0, focal_length_pix };

            const float ang = std::acos(_dot(center, pix) / 
                                        (_length(pix) * _length(center)));

            const float phi = std::atan2(pix[0], pix[1]);

            cam_coords.at(r, c, 0) = std::cos(ang);
            cam_coords.at(r, c, 1) = std::sin(ang) * std::cos(phi);
            cam_coords.at(r, c, 2) = std::sin(ang) * std::sin(phi);
        }
    }
}


void CPU_Range_Calculator::Convert_Camera_To_World_Coordinates(const Camera & cam, 
                                                               const Buffer & cam_coords, 
                                                               Buffer & world_coords)
{
    const auto sz = cam.focal_plane_dimensions();

    Buffer rot(3, 4);
    cam.get_rotation_matrix(rot.data());

    const float * rot_ptr = rot.data().get();

    const auto num_rows = std::get<0>(sz);
    const auto num_cols = std::get<1>(sz);
    for (auto r = 0; r < num_rows; r++) {
        for (auto c = 0; c < num_cols; c++) {
            const float * cam_coord = &(cam_coords.at(r, c, 0));

            world_coords.at(r, c, 0) = _dot(rot_ptr, cam_coord);
            world_coords.at(r, c, 1) = _dot(rot_ptr + 4, cam_coord);
            world_coords.at(r, c, 2) = _dot(rot_ptr + 8, cam_coord);
        }
    }
}


std::tuple<float, float, float> _sum(const std::tuple<float, float, float> & a, 
                                     const std::tuple<float, float, float> & b)
{
    return std::make_tuple(std::get<0>(a) + std::get<0>(b),
                           std::get<1>(a) + std::get<1>(b),
                           std::get<2>(a) + std::get<2>(b));
}


std::tuple<float, float, float> _mult(const std::tuple<float, float, float> & a, 
                                      const float f)
{
    return std::make_tuple(std::get<0>(a) * f,
                           std::get<1>(a) * f,
                           std::get<2>(a) * f);
}


float _clamp(const float val, const float low, const float hi)
{
    if (val < low) {
        return low;
    } else if (val > hi) {
        return hi;
    } else {
        return val;
    }
}


float _compute_range_for_pixel(const std::tuple<float, float, float> origin, 
                               const std::tuple<float, float, float> pv, 
                               const std::pair<float, float> bounds,
                               const Terrain & t,
                               const float max_error,
                               const float max_range)
{
    const float step = max_error / t.scale();
    const int iterations = static_cast<int>(std::ceil(max_range / max_error));

    const std::tuple<float, float, float> origin_pix = _mult(origin, 1.0 / t.scale());
    std::tuple<float, float, float> loc = origin_pix;

    for (auto i = 0; i < iterations; i++) {
        loc = _sum(loc, _mult(pv, step));

        const int r = _clamp(std::get<0>(loc), 0.0f, bounds.second);
        const int c = _clamp(std::get<1>(loc), 0.0f, bounds.first);
        const float height = t.data().at(r, c);

        if (std::get<2>(loc) <= height) {
            break;
        }
    }

    const std::tuple<float, float, float> diff_pix = _sum(loc, _mult(origin_pix, -1.0f));
    const float range_pixels = _length(&std::get<0>(diff_pix));

    return _clamp(t.scale() * range_pixels, 0.0f, max_range);
}


void CPU_Range_Calculator::Compute_Range(const Camera & cam, 
                                         const Terrain & t, 
                                         const Buffer & world_coords, 
                                         Buffer & rng)
{
    const auto sz = cam.focal_plane_dimensions();
    const auto num_rows = std::get<0>(sz);
    const auto num_cols = std::get<1>(sz);

    const std::tuple<float, float, float> origin = cam.position();
    const auto bounds = std::make_pair<float, float>(
        static_cast<float>(std::get<0>(t.data().size())),
        static_cast<float>(std::get<1>(t.data().size()))
    );

    const float max_range = t.scale() * std::get<0>(t.data().size()) * std::sqrt(3.0f);
    const float max_error = t.scale() / 5.0f;

    for (auto r = 0; r < num_rows; r++) {
        for (auto c = 0; c < num_cols; c++) {
            const auto pv = std::make_tuple(
                world_coords.at(r, c, 0),
                world_coords.at(r, c, 1),
                world_coords.at(r, c, 2)
            );

            rng.at(r, c) = _compute_range_for_pixel(origin, 
                                                    pv, 
                                                    bounds, 
                                                    t, 
                                                    max_error, 
                                                    max_range);
        }
    }
}

}
