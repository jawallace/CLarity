//! @file       cl_range_calculator.cc
//! @brief      Defines an implementation of Range_Calculator that uses OpenCL kernels
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT


// CLarity Imports
#include "buffer.h"
#include "camera.h"
#include "clarity_config.h"
#include "cl_range_calculator.h"
#include "cl_utils.h"
#include "device_buffer.h"
#include "range_calculator.h"
#include "terrain.h"

// Standard Imports
#include <algorithm>
#include <cmath>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

// Third-Party Imports
#include "cl.hpp"


namespace clarity
{


static std::map<std::string, std::string> _KERNEL_SOURCES {
    { "pix2cam",    KERNEL_DIR + "/pix_2_cam_coords.cl" },
    { "cam2world",  KERNEL_DIR + "/cam_2_world_coords.cl" },
    { "map_range",  KERNEL_DIR + "/map_range.cl" }
};


static bool _wrong_buffer_size(const Buffer & b, 
                               const std::tuple<uint32_t, uint32_t> & expected_size, 
                               const uint8_t expected_depth)
{
    const bool same_rows = std::get<0>(b.size()) == std::get<0>(expected_size);
    const bool same_cols = std::get<1>(b.size()) == std::get<1>(expected_size);
    
    return ! same_rows || ! same_cols || b.depth() != expected_depth;
}


static void _check_buffer_size(const Buffer & b, 
                               const std::tuple<uint32_t, uint32_t> & expected_size, 
                               const uint8_t expected_depth)
{
    if (_wrong_buffer_size(b, expected_size, expected_depth)) {
        std::stringstream msg;
        msg << "Invalid Argument. Expected buffer with size of (" << std::get<0>(expected_size);
        msg << ", " << std::get<1>(expected_size) << ", " << expected_depth << ") but got a ";
        msg << "buffer of size (" << std::get<0>(b.size()) << ", " << std::get<1>(b.size());
        msg << ", " << b.depth() << ")";

        throw std::invalid_argument(msg.str());
    }
}


CL_Range_Calculator::CL_Range_Calculator()
    : m_ctx()
    , m_devices()
    , m_device_queues()
    , m_camera_coords()
    , m_world_coords()
    , m_kernels()
    , m_device_idx(0)
{
    // Select a platform
    std::vector<cl::Platform> platforms = find_supported_platforms();

    if (platforms.size() == 0) {
        throw std::runtime_error("No OpenCL platforms are available.");
    }

    const cl::Platform & p = platforms[0];
    
    // Get the devices for the platform
    cl_int err = CL_SUCCESS;
    err = p.getDevices(CL_DEVICE_TYPE_ALL, &m_devices);
    
    if (err != CL_SUCCESS) {
        std::stringstream msg;
        msg << "Failed to get devices from the default platform (cl error = " << err << ")";
        throw std::runtime_error(msg.str());
    }

    // Create a context
    m_ctx = std::shared_ptr<cl::Context>(new cl::Context(m_devices, 
                                                         nullptr, 
                                                         nullptr, 
                                                         nullptr, 
                                                         &err));

    if (err != CL_SUCCESS) {
        std::stringstream msg;
        msg << "Failed to get create context from the default platform (cl error = " << err << ")";
        throw std::runtime_error(msg.str());
    }

   
    // Create a queue for each device
    for (const auto & d : m_devices) {
        m_device_queues.emplace_back(*m_ctx, d, 0, &err);

        if (err != CL_SUCCESS) {
            std::string device_name;
            d.getInfo(CL_DEVICE_NAME, &device_name);

            std::stringstream msg;
            msg << "Failed to create command queue for device (" << device_name << ") ";
            msg << "(cl error = " << err << ")";
            throw std::runtime_error(msg.str());
        }
    }

    // Construct the kernel collection
    m_kernels = std::unique_ptr<Kernel_Collection>(new Kernel_Collection(*m_ctx, _KERNEL_SOURCES));
}



CL_Range_Calculator::CL_Range_Calculator(const std::shared_ptr<cl::Context> ctx)
    : m_ctx(ctx)
    , m_devices()
    , m_device_queues()
    , m_camera_coords()
    , m_world_coords()
    , m_kernels()
    , m_device_idx(0)
{
    // Get the devices for the context
    m_ctx->getInfo(CL_CONTEXT_DEVICES, &m_devices); 

    // Create a queue for each device
    cl_int err = CL_SUCCESS;
    for (const auto & d : m_devices) {
        m_device_queues.emplace_back(*m_ctx, d, 0, &err);

        if (err != CL_SUCCESS) {
            std::string device_name;
            d.getInfo(CL_DEVICE_NAME, &device_name);

            std::stringstream msg;
            msg << "Failed to create command queue for device (" << device_name << ") ";
            msg << "(cl error = " << err << ")";
            throw std::runtime_error(msg.str());
        }
    }
    
    m_kernels = std::unique_ptr<Kernel_Collection>(new Kernel_Collection(*m_ctx, _KERNEL_SOURCES));
}


CL_Range_Calculator::~CL_Range_Calculator()
{
    // No-op
}


void CL_Range_Calculator::Calculate(const Camera & cam, const Terrain & t, Buffer & rng)
{
    const auto & fp_size = cam.focal_plane_dimensions();
    const auto rows = std::get<0>(fp_size);
    const auto cols = std::get<1>(fp_size);

    _check_buffer_size(rng, fp_size, 1);

    if (m_camera_coords == nullptr || _wrong_buffer_size(*m_camera_coords, fp_size, 3)) {
        m_camera_coords = std::unique_ptr<Device_Buffer>(new Device_Buffer(*m_ctx, rows, cols, 3));
    }

    run_pix2cam(cam, *m_camera_coords, false);

    if (m_world_coords == nullptr || _wrong_buffer_size(*m_world_coords, fp_size, 3)) {
        m_world_coords = std::unique_ptr<Device_Buffer>(new Device_Buffer(*m_ctx, rows, cols, 3));
    }

    run_cam2world(cam, *m_camera_coords, *m_world_coords, false);

    run_map_range(cam, t, *m_world_coords, rng, false);
}


void CL_Range_Calculator::Convert_Pixel_To_Camera_Coordinates(const Camera & cam, 
                                                              Buffer & cam_coords)
{
    run_pix2cam(cam, cam_coords, true);
}


void CL_Range_Calculator::run_pix2cam(const Camera & cam, Buffer & cam_coords, const bool copy)
{
    const auto & fp_size = cam.focal_plane_dimensions();
    const auto rows = std::get<0>(fp_size);
    const auto cols = std::get<1>(fp_size);

    const auto sz = std::make_tuple(static_cast<uint32_t>(rows), static_cast<uint32_t>(cols));
    _check_buffer_size(cam_coords, sz, 3); 

    const cl::CommandQueue & queue = m_device_queues[m_device_idx];
    cl::Kernel & kernel = m_kernels->get("pix2cam");

    // Set up arguments
    const cl_float3 boresight {{ rows / 2.f, cols / 2.f, cam.focal_length() }};
    Device_Buffer & cam_coords_db = dynamic_cast<Device_Buffer &>(cam_coords);
    
    kernel.setArg(0, boresight);
    kernel.setArg(1, cols);
    kernel.setArg(2, cam_coords_db.get_cl_buffer());
    
    cl_int err = CL_SUCCESS;
    err = queue.enqueueNDRangeKernel(kernel, 
                                     cl::NullRange, 
                                     cl::NDRange(rows, cols), 
                                     cl::NullRange);

    if (err != CL_SUCCESS) {
        std::stringstream msg;
        msg << "Failed to enqueue cam2world kernel (cl error = " << err << ")";
        throw std::runtime_error(msg.str());
    }

    if (copy) {
        // Copy from device to host
        cam_coords_db.from_device();
    }
}


void CL_Range_Calculator::Convert_Camera_To_World_Coordinates(const Camera & cam, 
                                                              const Buffer & cam_coords, 
                                                              Buffer & world_coords)
{
    run_cam2world(cam, cam_coords, world_coords, true);
}


void CL_Range_Calculator::run_cam2world(const Camera & cam, 
                                        const Buffer & cam_coords, 
                                        Buffer & world_coords,
                                        const bool copy)
{
    const auto & fp_size = cam.focal_plane_dimensions();
    const auto rows = std::get<0>(fp_size);
    const auto cols = std::get<1>(fp_size);

    const auto sz = std::make_tuple(static_cast<uint32_t>(rows), static_cast<uint32_t>(cols));
    _check_buffer_size(world_coords, sz, 3); 

    const cl::CommandQueue & queue = m_device_queues[m_device_idx];
    cl::Kernel & kernel = m_kernels->get("cam2world");

    // TODO compute rotation matrix
    const Device_Buffer & cam_coords_db = dynamic_cast<const Device_Buffer &>(cam_coords);
    Device_Buffer & world_coords_db = dynamic_cast<Device_Buffer &>(world_coords);
    kernel.setArg(0, cam_coords_db.get_cl_buffer());
    kernel.setArg(1, cl::Buffer());
    kernel.setArg(2, cols);
    kernel.setArg(3, world_coords_db);

    cl_int err = CL_SUCCESS;
    err = queue.enqueueNDRangeKernel(kernel, 
                                     cl::NullRange, 
                                     cl::NDRange(rows, cols), 
                                     cl::NullRange);

    if (err != CL_SUCCESS) {
        std::stringstream msg;
        msg << "Failed to enqueue cam2world kernel (cl error = " << err << ")";
        throw std::runtime_error(msg.str());
    }

    if (copy) {
        // Copy from device to host
        world_coords_db.from_device();
    }
}


void CL_Range_Calculator::Compute_Range(const Camera & cam, 
                                        const Terrain & t, 
                                        const Buffer & world_coords, 
                                        Buffer & rng)
{
    run_map_range(cam, t, world_coords, rng, true);
}


//! @brief  See Range_Calculator::Compute_Range
void CL_Range_Calculator::run_map_range(const Camera & cam, 
                                        const Terrain & t, 
                                        const Buffer & world_coords, 
                                        Buffer & rng,
                                        const bool copy)
{
    const auto & fp_size = cam.focal_plane_dimensions();
    const auto rows = std::get<0>(fp_size);
    const auto cols = std::get<1>(fp_size);

    const auto sz = std::make_tuple(static_cast<uint32_t>(rows), static_cast<uint32_t>(cols));
    _check_buffer_size(world_coords, sz, 3); 

    const cl::CommandQueue & queue = m_device_queues[m_device_idx];
    cl::Kernel & kernel = m_kernels->get("cam2world");

    // Set up args
    const auto & pos = cam.position();
    const cl_float3 origin = {{ std::get<0>(pos), std::get<1>(pos), std::get<2>(pos) }};
    const Device_Buffer & world_coords_db = dynamic_cast<const Device_Buffer &>(world_coords);
    const Device_Buffer & terrain_db = dynamic_cast<const Device_Buffer &>(t.data());
    Device_Buffer & range_db = dynamic_cast<Device_Buffer &>(rng);
    const auto & terrain_size = t.data().size();
    const cl_float2 bounds = {{ static_cast<float>(std::get<0>(terrain_size)), 
                                static_cast<float>(std::get<1>(terrain_size)) }};

    kernel.setArg(0, origin);
    kernel.setArg(1, world_coords_db.get_cl_buffer());
    kernel.setArg(2, terrain_db.get_cl_buffer());
    kernel.setArg(3, t.scale());
    kernel.setArg(4, t.scale() * std::sqrt(3.0f));
    kernel.setArg(5, t.scale() / 2.0f);
    kernel.setArg(6, bounds);
    kernel.setArg(7, range_db.get_cl_buffer());

    cl_int err = CL_SUCCESS;
    err = queue.enqueueNDRangeKernel(kernel, 
                                     cl::NullRange, 
                                     cl::NDRange(rows, cols), 
                                     cl::NullRange);

    if (err != CL_SUCCESS) {
        std::stringstream msg;
        msg << "Failed to enqueue map_rng kernel (cl error = " << err << ")";
        throw std::runtime_error(msg.str());
    }

    if (copy) {
        // Copy from device to host
        range_db.from_device();
    }
}


//! @brief  Get the OpenCL devices that can be used
std::vector<cl::Device> & CL_Range_Calculator::get_devices()
{
    return m_devices;
}


//! @brief  Set the OpenCL device to be used
void CL_Range_Calculator::use_device(const uint8_t device_idx)
{
    if (device_idx >= m_devices.size()) {
        std::stringstream msg;
        msg << "Invalid device index (" << device_idx << "). Only (" << m_devices.size() << ") ";
        msg << "are available.";
        throw std::out_of_range(msg.str());
    }

    m_device_idx = device_idx;
}


}
