//! @file       test_cl_range_calculator.cc
//! @brief      Unit tests for the Device_Buffer type
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// CLarity Imports
#include "cl_range_calculator.h"
#include "cl_utils.h"
#include "device_buffer.h"

// Standard Imports
#include <memory>
#include <cmath>

// Third-Party Imports
#include "cl.hpp"
#include "gtest/gtest.h"

namespace
{

using namespace clarity;


TEST(cl_range_calculator, pix2cam)
{
    std::shared_ptr<cl::Context> ctx = get_context();

    Camera cam(90 * M_PI / 180, 256, 256);
    Device_Buffer b(*ctx, 256, 256, 3);

    CL_Range_Calculator calculator(ctx);

    // zero buffer 
    for (auto i = 0; i < 256; i++) {
        for (auto j = 0; j < 256; j++) {
            b.at(i, j, 0) = 0.0; 
            b.at(i, j, 1) = 0.0; 
            b.at(i, j, 2) = 0.0; 
        }
    }

    calculator.Convert_Pixel_To_Camera_Coordinates(cam, b);

    // check that buffer is not zero
    int count = 0;
    for (auto i = 0; i < 256; i++) {
        for (auto j = 0; j < 256; j++) {
            auto x = b.at(i, j, 0);
            auto y = b.at(i, j, 1);
            auto z = b.at(i, j, 2);

            if (x == 0.0 && y == 0.0 && z == 0.0) {
                count++;
            }
        }
    }

    ASSERT_EQ(0, count);
}


TEST(cl_range_calculator, cam2world_no_rotation)
{
    std::shared_ptr<cl::Context> ctx = get_context();

    Camera cam(90 * M_PI / 180, 256, 256);
    Device_Buffer b(*ctx, 256, 256, 4);
    Device_Buffer b2(*ctx, 256, 256, 4);

    CL_Range_Calculator calculator(ctx);

    // set up test input data, clear output buffer
    for (auto i = 0; i < 256; i++) {
        for (auto j = 0; j < 256; j++) {
            // sensor coords all look down boresight 
            b.at(i, j, 0) = 1.0; 
            b.at(i, j, 1) = 0.0; 
            b.at(i, j, 2) = 0.0; 

            b2.at(i, j, 0) = 0.0; 
            b2.at(i, j, 1) = 0.0; 
            b2.at(i, j, 2) = 0.0; 
        }
    }

    // cam has no rotation, so result should still be (1, 0, 0)
    calculator.Convert_Camera_To_World_Coordinates(cam, b, b2);

    // check that buffer is not zero
    for (auto i = 0; i < 256; i++) {
        for (auto j = 0; j < 256; j++) {
            auto x = b2.at(i, j, 0);
            auto y = b2.at(i, j, 1);
            auto z = b2.at(i, j, 2);

            ASSERT_FLOAT_EQ(x, 1.0);
            ASSERT_FLOAT_EQ(y, 0.0);
            ASSERT_FLOAT_EQ(z, 0.0);
        }
    }
}


TEST(cl_range_calculator, cam2world_90_deg_yaw)
{
    std::shared_ptr<cl::Context> ctx = get_context();

    Camera cam(90 * M_PI / 180, 256, 256);
    Device_Buffer b(*ctx, 256, 256, 4);
    Device_Buffer b2(*ctx, 256, 256, 4);

    CL_Range_Calculator calculator(ctx);

    // set up test input data, clear output buffer
    for (auto i = 0; i < 256; i++) {
        for (auto j = 0; j < 256; j++) {
            // sensor coords all look down boresight 
            b.at(i, j, 0) = 1.0; 
            b.at(i, j, 1) = 0.0; 
            b.at(i, j, 2) = 0.0; 
            b.at(i, j, 3) = 0.0; 

            b2.at(i, j, 0) = 0.0; 
            b2.at(i, j, 1) = 0.0; 
            b2.at(i, j, 2) = 0.0; 
            b2.at(i, j, 3) = 0.0; 
        }
    }

    cam.set_yaw(90 * M_PI / 180);

    // cam has 90 deg rotation in yaw, so result should be (0, 1, 0)
    calculator.Convert_Camera_To_World_Coordinates(cam, b, b2);

    for (auto i = 0; i < 256; i++) {
        for (auto j = 0; j < 256; j++) {
            ASSERT_NEAR(b2.at(i, j, 0), 0.0, 0.0005) << i << ", " << j;
            ASSERT_NEAR(b2.at(i, j, 1), 1.0, 0.0005) << i << ", " << j;
            ASSERT_NEAR(b2.at(i, j, 2), 0.0, 0.0005) << i << ", " << j;
            ASSERT_NEAR(b2.at(i, j, 3), 0.0, 0.0005) << i << ", " << j;
        }
    }
}


TEST(cl_range_calculator, cam2world_90_deg_pitch)
{
    std::shared_ptr<cl::Context> ctx = get_context();

    Camera cam(90 * M_PI / 180, 256, 256);
    Device_Buffer b(*ctx, 256, 256, 4);
    Device_Buffer b2(*ctx, 256, 256, 4);

    CL_Range_Calculator calculator(ctx);

    // set up test input data, clear output buffer
    for (auto i = 0; i < 256; i++) {
        for (auto j = 0; j < 256; j++) {
            // sensor coords all look down boresight 
            b.at(i, j, 0) = 1.0; 
            b.at(i, j, 1) = 0.0; 
            b.at(i, j, 2) = 0.0; 

            b2.at(i, j, 0) = 0.0; 
            b2.at(i, j, 1) = 0.0; 
            b2.at(i, j, 2) = 0.0; 
        }
    }

    cam.set_pitch(90 * M_PI / 180);

    // cam has 90 deg pitch, so result should be (0, 0, -1)
    calculator.Convert_Camera_To_World_Coordinates(cam, b, b2);

    // check that buffer is not zero
    for (auto i = 0; i < 256; i++) {
        for (auto j = 0; j < 256; j++) {
            auto x = b2.at(i, j, 0);
            auto y = b2.at(i, j, 1);
            auto z = b2.at(i, j, 2);

            ASSERT_NEAR(x, 0.0, 0.0005);
            ASSERT_NEAR(y, 0.0, 0.0005);
            ASSERT_NEAR(z, -1.0, 0.0005);
        }
    }
}


TEST(cl_range_calculator, calculate)
{
    std::shared_ptr<cl::Context> ctx = get_context();

    Camera cam(90 * M_PI / 180, 256, 256);
    Device_Buffer b(*ctx, 256, 256);
    auto tb = std::make_shared<Device_Buffer>(*ctx, 512, 512);
    Terrain t(tb, 30.0);

    CL_Range_Calculator calculator(ctx);

    // zero buffer 
    for (auto i = 0; i < 256; i++) {
        for (auto j = 0; j < 256; j++) {
            tb->at(i, j, 0) = 0.0; 
            tb->at(i, j, 1) = 0.0; 
            tb->at(i, j, 2) = 0.0; 
            
            b.at(i, j) = 0.0; 
        }
    }

    //calculator.Calculate(cam, t, b);

    // check that buffer is not zero
    int count = 0;
    for (auto i = 0; i < 256; i++) {
        for (auto j = 0; j < 256; j++) {
            auto x = b.at(i, j, 0);

            if (x == 0.0) {
                count++;
            }
        }
    }

    ASSERT_EQ(0, count);
}
}
