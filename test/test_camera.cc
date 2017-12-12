//! @file       test_camera.cc
//! @brief      Unit tests for the camera type
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// CLarity Imports
#include "buffer.h"
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
    }
}


TEST(camera, focal_length)
{
    Camera cam(M_PI * 120.0 / 180.0, 200, 200);

    ASSERT_FLOAT_EQ(57.735027, cam.focal_length());
}


TEST(camera, get_rot)
{
    Camera cam(M_PI * 120.0 / 180.0, 200, 200);
    Buffer rot(3, 4);
    for (auto i = 0; i < 3; i++) {
      for (auto j = 0; j < 3; j++) {
        rot.at(i, j) = 0.0;
      }
    }

    cam.set_yaw(M_PI * 90.0 / 180.0);

    cam.get_rotation_matrix(rot.data());

    ASSERT_NEAR(rot.at(0, 0), 0.0, 0.00005);
    ASSERT_NEAR(rot.at(0, 1), -1.0, 0.00005);
    ASSERT_NEAR(rot.at(0, 2), 0.0, 0.00005);

    ASSERT_NEAR(rot.at(1, 0), 1.0, 0.00005);
    ASSERT_NEAR(rot.at(1, 1), 0.0, 0.00005);
    ASSERT_NEAR(rot.at(1, 2), 0.0, 0.00005);

    ASSERT_NEAR(rot.at(2, 0), 0.0, 0.00005);
    ASSERT_NEAR(rot.at(2, 1), 0.0, 0.00005);
    ASSERT_NEAR(rot.at(2, 2), 1.0, 0.00005);
}


TEST(camera, get_rot_combination)
{
    Camera cam(M_PI * 120.0 / 180.0, 200, 200);
    Buffer rot(3, 4);
    for (auto i = 0; i < 3; i++) {
      for (auto j = 0; j < 3; j++) {
        rot.at(i, j) = 0.0;
      }
    }

    cam.set_yaw(M_PI * 45.0 / 180.0);
    cam.set_pitch(M_PI * 45.0 / 180.0);

    cam.get_rotation_matrix(rot.data());

    ASSERT_NEAR(rot.at(0, 0), 0.5, 0.00005);
    ASSERT_NEAR(rot.at(0, 1), -.5, 0.00005);
    ASSERT_NEAR(rot.at(0, 2), .7071068, 0.00005);

    ASSERT_NEAR(rot.at(1, 0), .7071068, 0.00005);
    ASSERT_NEAR(rot.at(1, 1), .7071068, 0.00005);
    ASSERT_NEAR(rot.at(1, 2), 0.0, 0.00005);

    ASSERT_NEAR(rot.at(2, 0), -0.5, 0.00005);
    ASSERT_NEAR(rot.at(2, 1), 0.5, 0.00005);
    ASSERT_NEAR(rot.at(2, 2), .7071068, 0.00005);
}

}


