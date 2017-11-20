//! @file       test_cl_utils.cc
//! @brief      Unit tests for the cl_utils.cc module
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// CLarity imports
#include "cl_utils.h"
#include "clarity_config.h"

// Standard imports
#include <iostream>
#include <map>
#include <vector>

// Third-party imports
#include <cl.hpp>
#include <gtest/gtest.h>


namespace 
{

using namespace clarity;


TEST(cl_utils, supported_platforms)
{
    std::vector<cl::Platform> platforms = find_supported_platforms();

    const unsigned long expected = 0;
    ASSERT_LT(expected, platforms.size()) << "Wrong number of platfoms found fail";
}


TEST(cl_utils, kernel_collection)
{
    std::vector<cl::Platform> platforms = find_supported_platforms();
    std::vector<cl::Device> devices; 
    platforms[0].getDevices(CL_DEVICE_TYPE_DEFAULT, &devices);

    std::map<std::string, std::string> files { 
        { "simple_kernel", KERNEL_DIR + "/simple_kernel.cl" } 
    };
    
    cl_int err;
    cl::Context ctx(devices, nullptr, nullptr, nullptr, &err);

    ASSERT_EQ(CL_SUCCESS, err) << "Failed to get context";
    
    Kernel_Collection kcollect(ctx, files);

    cl::Kernel & k = kcollect.get("simple_kernel");

    std::string kernel_name;
    k.getInfo(CL_KERNEL_FUNCTION_NAME, &kernel_name);
    ASSERT_NE(kernel_name.find("simple_kernel"), std::string::npos) << "get returned the wrong kernel";
}


TEST(cl_utils, kernel_compliation)
{
    std::vector<cl::Platform> platforms = find_supported_platforms();
    std::vector<cl::Device> devices; 
    platforms[0].getDevices(CL_DEVICE_TYPE_DEFAULT, &devices);

    std::map<std::string, std::string> files { 
        { "pix2cam", KERNEL_DIR + "/pix_2_cam_coords.cl" },
        { "cam2world", KERNEL_DIR + "/cam_2_world_coords.cl" },
        { "map_range", KERNEL_DIR + "/map_range.cl" }
    };
    
    cl_int err;
    cl::Context ctx(devices, nullptr, nullptr, nullptr, &err);

    ASSERT_EQ(CL_SUCCESS, err) << "Failed to get context";

    try {
        Kernel_Collection kcollect(ctx, files);
    } catch(const std::exception & e) {
        std::cerr << e.what() << std::endl;
        FAIL();
    }
}

}

