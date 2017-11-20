//! @file       cl_range_calculator.h
//! @brief      Declares an implementation of Range_Calculator that uses OpenCL kernels
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

#pragma once

// CLarity Imports
#include "buffer.h"
#include "camera.h"
#include "cl_utils.h"
#include "device_buffer.h"
#include "range_calculator.h"
#include "terrain.h"

// Standard Imports
#include <memory>
#include <vector>

// Third-Party Imports
#include "cl.hpp"


namespace clarity
{

//! @brief  Implementation of Range_Calculator that utilizes OpenCL to perform an efficent
//!         range calculation.
class CL_Range_Calculator : public Range_Calculator
{

    //! @brief  Default constructor.
    //!
    //! @detail Selects first platform/context and a single device for computation.
    CL_Range_Calculator();


    //! @brief  Constructor
    //!
    //! @detail Use the given OpenCL context. Allows the client to decide on a platform,
    //!         context, and device.
    //!
    //! @param[in]  ctx     The OpenCL context to use
    CL_Range_Calculator(const std::shared_ptr<cl::Context> ctx);


    //! @brief  Destructor
    ~CL_Range_Calculator();


    //! @brief  Deleted copy constructor
    CL_Range_Calculator(const CL_Range_Calculator & other) = delete;


    //! @brief  Deleted assignment operator
    CL_Range_Calculator & operator=(const CL_Range_Calculator & other) = delete;


    //! @brief  See Range_Calculator::Calculate
    void Calculate(const Camera & cam, const Terrain & t, Buffer & rng);


    //! @brief  See Range_Calculator::Convert_Pixel_To_Camera_Coordinates
    void Convert_Pixel_To_Camera_Coordinates(const Camera & cam, Buffer & cam_coords);


    //! @brief  See Range_Calculator::Convert_Camera_To_World_Coordinates
    void Convert_Camera_To_World_Coordinates(const Camera & cam, 
                                             const Buffer & cam_coords, 
                                             Buffer & world_coords);


    //! @brief  See Range_Calculator::Compute_Range
    void Compute_Range(const Camera & cam, 
                       const Terrain & t, 
                       const Buffer & world_coords, 
                       Buffer & rng);


    //! @brief  Get the OpenCL devices that can be used
    std::vector<cl::Device> & get_devices();


    //! @brief  Set the OpenCL device to be used
    //!
    //! @param[in]  device_idx  index of a device returned by CL_Range_Calculator::get_devices
    //!                         that specifies the device to use
    void use_device(const uint8_t device_idx);

private:
    
    void run_pix2cam(const Camera & cam, Buffer & cam_coords, const bool copy);


    void run_cam2world(const Camera & cam, 
                       const Buffer & cam_coords, 
                       Buffer & world_coords, 
                       const bool copy);


    void run_map_range(const Camera & cam, 
                       const Terrain & t, 
                       const Buffer & world_coords, 
                       Buffer & rng,
                       bool copy);

    //! The OpenCL context to use
    std::shared_ptr<cl::Context> m_ctx;

    //! The available devices to use
    std::vector<cl::Device> m_devices;

    //! The command queues to utilize
    std::vector<cl::CommandQueue> m_device_queues;

    //! The device buffer for the Camera coordinates
    std::unique_ptr<Device_Buffer> m_camera_coords;

    //! The device buffer for the world coordinates
    std::unique_ptr<Device_Buffer> m_world_coords;

    //! The kernels for the range calculation
    std::unique_ptr<Kernel_Collection> m_kernels;

    //! The index of the device to use in m_devices
    uint8_t m_device_idx;
};

}
