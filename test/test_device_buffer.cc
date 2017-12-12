//! @file       test_device_buffer.cc
//! @brief      Unit tests for the Device_Buffer type
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// CLarity Imports
#include "cl_utils.h"
#include "device_buffer.h"

// Standard Imports
#include <memory>

// Third-Party Imports
#include "cl.hpp"
#include "gtest/gtest.h"

namespace
{

using namespace clarity;


TEST(device_buffer, copy)
{
    std::shared_ptr<cl::Context> ctx = get_context();

    cl_int err = CL_SUCCESS;
    cl::CommandQueue q(*ctx, 0, &err);
    ASSERT_EQ(CL_SUCCESS, err);

    Device_Buffer b(*ctx, 256, 256);

    b.to_device(&q);
    b.from_device(&q);
    cl::finish();
}

}
