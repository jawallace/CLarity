//! @file       device_buffer.cc
//! @brief      Defines the Device_Buffer type, which is a subclass of Buffer that manages the 
//!             corresponding cl::Buffer
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// CLarity Imports
#include "buffer.h"
#include "device_buffer.h"

// Standard Imports
#include <sstream>
#include <stdexcept>

// Third-Party Imports
#include "cl.hpp"

namespace clarity
{

Device_Buffer::Device_Buffer(const cl::Context & ctx, 
                             const uint32_t rows, 
                             const uint32_t cols, 
                             const uint8_t depth,
                             const bool read_only)
    : Buffer(rows, cols, depth)
    , m_ctor_err(CL_SUCCESS)
    , m_cl_buffer(ctx, 
                  m_data.get(), 
                  m_data.get() + (m_rows * m_cols * m_depth),
                  read_only, true, &m_ctor_err)
{
    if (m_ctor_err != CL_SUCCESS) {
        std::stringstream msg;
        msg << "Failed to allocated buffer (cl error = " << m_ctor_err << ")";
        throw std::runtime_error(msg.str());
    }
}


Device_Buffer::Device_Buffer(const Buffer & b, 
                             const cl::Context & ctx, 
                             const bool read_only)
    : Buffer(b)
    , m_ctor_err(CL_SUCCESS)
    , m_cl_buffer(ctx, 
                  m_data.get(), 
                  m_data.get() + (m_rows * m_cols * m_depth),
                  read_only, true, &m_ctor_err)
{
    if (m_ctor_err != CL_SUCCESS) {
        std::stringstream msg;
        msg << "Failed to allocated buffer (cl error = " << m_ctor_err << ")";
        throw std::runtime_error(msg.str());
    }
}


Device_Buffer::~Device_Buffer()
{
    // No-op
}


Device_Buffer::Device_Buffer(const Device_Buffer & other)
    : Buffer(other)
    , m_ctor_err(CL_SUCCESS)
    , m_cl_buffer(other.m_cl_buffer)
{
    // No-op
}


Device_Buffer & Device_Buffer::operator=(const Device_Buffer & other)
{
    if (this == &other) {
        return *this;
    }

    Buffer::operator=(other);
    m_ctor_err = other.m_ctor_err;
    m_cl_buffer = other.m_cl_buffer;

    return *this;
}


void Device_Buffer::from_device(const cl::CommandQueue * queue)
{
    cl_int err = CL_SUCCESS;
    if (queue == nullptr) {
        err = cl::copy(m_cl_buffer, 
                       m_data.get(),  
                       m_data.get() + (m_rows * m_cols * m_depth));
    } else {
        const auto start = m_data.get();
        const auto end = m_data.get() + (m_rows * m_cols * m_depth);
        err = cl::copy(*queue,
                       m_cl_buffer, 
                       start,
                       end);
    }

    if (err != CL_SUCCESS) {
        std::stringstream msg;
        msg << "Failed to get buffer from device (cl error = " << err << ")";
        throw std::runtime_error(msg.str());
    }
}


void Device_Buffer::to_device(const cl::CommandQueue * queue)
{
    cl_int err = CL_SUCCESS;
    if (queue == nullptr) {
        err = cl::copy(m_data.get(),  
                       m_data.get() + (m_rows * m_cols * m_depth), 
                       m_cl_buffer);
    } else {
        err = cl::copy(*queue,
                       m_data.get(),  
                       m_data.get() + (m_rows * m_cols * m_depth), 
                       m_cl_buffer);
    }

    if (err != CL_SUCCESS) {
        std::stringstream msg;
        msg << "Failed to send buffer to device (cl error = " << err << ")";
        throw std::runtime_error(msg.str());
    }
}

}
