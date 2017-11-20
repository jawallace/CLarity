//! @file       device_buffer.h
//! @brief      Declares the Device_Buffer type, which is a subclass of Buffer that manages the 
//!             corresponding cl::Buffer
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

#pragma once

// CLarity Imports
#include "buffer.h"

// Standard Imports

// Third-Party Imports
#include "cl.hpp"

namespace clarity 
{

//! @brief  Defines a subtype of Buffer that has facilities for buffering the data to/from an
//!         OpenCL device.
class Device_Buffer : public Buffer
{
public:

    //! @brief  Construct a Device_Buffer
    //! 
    //! @param[in]     ctx         The OpenCL context to use
    //! @param[in]     rows        the number of rows in the Buffer
    //! @param[in]     cols        The number of cols in the Buffer
    //! @param[in]     depth       the depth of the Buffer
    //! @param[in]     read_only   whether the buffer is read only on the device
    Device_Buffer(const cl::Context & ctx, 
                  const uint32_t rows, 
                  const uint32_t cols, 
                  const uint8_t depth = 1,
                  const bool read_only = false);


    //! @brief  Convert a Buffer to a Device_Buffer
    //! 
    //! @param[in]      buffer      the Buffer to convert to a Device_Buffer
    //! @param[in]     ctx         The OpenCL context to use
    //! @param[in]      read_only   whether the buffer is read only on the device
    Device_Buffer(const Buffer & b, const cl::Context & ctx, const bool read_only = false);


    //! @brief  Destructor for the Device_Buffer
    ~Device_Buffer();


    //! @brief  Copy Constructor for a Device_Buffer
    Device_Buffer(const Device_Buffer & other);


    //! @brief  Assignment Operator for a Device_Buffer
    Device_Buffer & operator=(const Device_Buffer & other);


    //! @brief  Copy the data from the device to the host buffer.
    //!
    //! @detail This call is blocking
    //!
    //! @param[in]  queue   Optinal command queue to use. The default will be used otherwise
    void from_device(const cl::CommandQueue * queue = nullptr);


    //! @brief  Copy the data from the host to the device buffer.
    //!
    //! @detail This call is blocking
    //!
    //! @param[in]  queue   Optinal command queue to use. The default will be used otherwise
    void to_device(const cl::CommandQueue * queu = nullptr);


    inline const cl::Buffer & get_cl_buffer() const { return m_cl_buffer; }

private:
    //! An OpenCL error indicator for the constructor
    cl_int m_ctor_err;

    //! The OpenCL buffer
    cl::Buffer m_cl_buffer;
};

}
