//! @file       cl_utils.h
//! @brief      Declares utility functions for OpenCL management
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

#pragma once

// CLarity imports

// Standard imports
#include <map>
#include <memory>
#include <string>
#include <vector>

// Third-party imports
#include "cl.hpp"


namespace clarity 
{

//! @brief  Enumeration of target platforms. Allows selection of device class to use
enum Execution_Target
{
    CPU = 0,
    GPU = 1
};


//! @brief      Find all platforms that support the CLarity OpenCL requirements
//! 
//! @detail     Returns a vector of supported platforms
std::vector<cl::Platform> find_supported_platforms();



//! @brief      Simple wrapper around cl::Program to manage construction of program and retrieve
//!             kernels.
class Kernel_Collection
{
public:

    //! @brief  Cosntructor for the Kernel_Collection class
    //!
    //! @param[in]  ctx             the OpenCL Context to use to construct the kernels
    //! @param[in]  kernel_files    a mapping from kernel name to implementation file path. Paths
    //!                             can be relative to the pwd or absolute paths
    Kernel_Collection(const cl::Context & ctx, 
                      const std::map<std::string, std::string> & kernel_files);


    //! @brief  Destructor for the Kernel_Collection type
    ~Kernel_Collection();


    //! @brief  Deleted copy constructor
    Kernel_Collection(const Kernel_Collection & other) = delete;


    //! @brief  Deleted assignment operator
    Kernel_Collection & operator=(const Kernel_Collection & other) = delete;


    //! @brief  Retrieve a kernel based on the name
    cl::Kernel & get(const std::string & kernel_name);

private:
    //! The program that encapsulates the kernels
    std::unique_ptr<cl::Program> m_program;

    //! The kernels in the program
    std::map<std::string, cl::Kernel> m_kernels;
};

}

