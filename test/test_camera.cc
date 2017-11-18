//! @file       test_camera.cc
//! @brief      Unit tests for the camera type
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
#include "gtest/gtest.h"

namespace {

using namespace clarity;

TEST(camera, construction)
{
    Camera cam(60.0, 200, 200);

    ASSERT_FLOAT_EQ(60.0, cam.fov()) << "Camera fov was not correct";

    const auto & sz = cam.focal_plane_dimensions();
    uint16_t dim = 200;
    ASSERT_EQ(dim, sz.first) << "Camera rows were not correct";
    ASSERT_EQ(dim, sz.second) << "Camera cols were not correct";

    const auto & pos = cam.position();
    ASSERT_EQ(0.0, std::get<0>(pos));
    ASSERT_EQ(0.0, std::get<1>(pos));
    ASSERT_EQ(0.0, std::get<2>(pos));
    
    const auto & pv = cam.pointing_vector();
    ASSERT_EQ(1.0, std::get<0>(pv));
    ASSERT_EQ(0.0, std::get<1>(pv));
    ASSERT_EQ(0.0, std::get<2>(pv));
}


TEST(camera, assignment_and_copy)
{
    Camera cam(60.0, 200, 200);

    {
        Camera cam2(cam);
        const auto & sz = cam2.focal_plane_dimensions();
        uint16_t dim = 200;
        ASSERT_EQ(dim, sz.first) << "Camera rows were not correct";
        ASSERT_EQ(dim, sz.second) << "Camera cols were not correct";

        const auto & pos = cam2.position();
        ASSERT_EQ(0.0, std::get<0>(pos));
        ASSERT_EQ(0.0, std::get<1>(pos));
        ASSERT_EQ(0.0, std::get<2>(pos));
        
        const auto & pv = cam2.pointing_vector();
        ASSERT_EQ(1.0, std::get<0>(pv));
        ASSERT_EQ(0.0, std::get<1>(pv));
        ASSERT_EQ(0.0, std::get<2>(pv));
    }

    {
        Camera cam3(25.0, 100, 100);
        cam3 = cam;
        const auto & sz = cam3.focal_plane_dimensions();
        uint16_t dim = 200;
        ASSERT_EQ(dim, sz.first) << "Camera rows were not correct";
        ASSERT_EQ(dim, sz.second) << "Camera cols were not correct";

        const auto & pos = cam3.position();
        ASSERT_EQ(0.0, std::get<0>(pos));
        ASSERT_EQ(0.0, std::get<1>(pos));
        ASSERT_EQ(0.0, std::get<2>(pos));
        
        const auto & pv = cam3.pointing_vector();
        ASSERT_EQ(1.0, std::get<0>(pv));
        ASSERT_EQ(0.0, std::get<1>(pv));
        ASSERT_EQ(0.0, std::get<2>(pv));
    }
}


TEST(camera, focal_length)
{
    Camera cam(M_PI * 120.0 / 180.0, 200, 200);

    ASSERT_FLOAT_EQ(57.735027, cam.focal_length());
}

}


