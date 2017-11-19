//! @file       cl_utils.cc
//! @brief      Defines utility functions for OpenCL management
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// CLarity imports
#include "cl_utils.h"

// Standard imports
#include <fstream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

// Third-party imports
#include <cl.hpp>


namespace clarity
{


//! @brief  Utility to check if the platform is supported
static bool _is_supported_platform(const cl::Platform & plat)
{
	const std::string platver = plat.getInfo<CL_PLATFORM_VERSION>();
	return platver.find("OpenCL 1.2") == std::string::npos;
}


std::vector<cl::Platform> find_supported_platforms()
{
    cl_int err = CL_SUCCESS;
    std::vector<cl::Platform> platforms;
    err = cl::Platform::get(&platforms);
    if (err != CL_SUCCESS) {
        throw std::runtime_error("Failed to get platforms");
    }

	platforms.erase(std::remove_if(std::begin(platforms),
								   std::end(platforms),
								   _is_supported_platform),
					std::end(platforms));

	return platforms;
}


//! @brief  Utility to read the entirety of a file
static std::string _read_source(const std::string & path)
{
    std::ifstream in(path);

    if (! in.good()) {
        std::stringstream msg;
        msg << "Failed to build kernel. Could not open kernel file at (" << path << ")";
        throw std::invalid_argument(msg.str());
    }

    std::stringstream src;
    src << in.rdbuf();

    return src.str();
}



Kernel_Collection::Kernel_Collection(const cl::Context & ctx, 
									 const std::map<std::string, std::string> & kernel_files)
	: m_program(nullptr), m_kernels()
{
    // Collect the sources
    std::stringstream src;
	for (auto & entry : kernel_files) {
        src << _read_source(entry.second) << std::endl;
    }

    // Create and build the program
    cl_int err = CL_SUCCESS;
    m_program = std::unique_ptr<cl::Program>(new cl::Program(ctx, src.str(), true, &err));

    // Make sure it build successfully
    if (err != CL_SUCCESS) {
        std::vector<cl::Device> ctx_devices;
        ctx.getInfo(CL_CONTEXT_DEVICES, & ctx_devices);
       
        // Build error message
        std::stringstream msg;
        msg << "Error - program did not build succesfully (cl error = " << err << ")." << std::endl;
        for (auto & d : ctx_devices) {
            // Get device name
            std::string device_name;
            d.getInfo(CL_DEVICE_NAME, &device_name);

            // Get build log for device
            std::string build_log;
            m_program->getBuildInfo<std::string>(d, CL_PROGRAM_BUILD_LOG, &build_log);

            // Append meesage
            msg << "Build Log: (" << device_name << ")" << std::endl;
            msg << build_log << std::endl;
        }

        throw std::runtime_error(msg.str());
    }

    err = CL_SUCCESS;
    for (auto & entry : kernel_files) {
        m_kernels[entry.first] = cl::Kernel(*m_program, entry.first.c_str(), &err);

        if (err != CL_SUCCESS) {
            std::stringstream msg;
            msg << "Error - failed to construct Kernel from program (cl error = " << err << ")";
            throw std::runtime_error(msg.str());
        }
    }
}


Kernel_Collection::~Kernel_Collection()
{
    m_kernels.clear();
}


cl::Kernel & Kernel_Collection::get(const std::string & kernel_name)
{
    return m_kernels.at(kernel_name);
}


}
