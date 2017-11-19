//! @file       simple_kernel.cl
//! @brief      Simple kernel definition for testing purposes
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT


__kernel void simple_kernel(__global float * a, __global float * b)
{
    const int i = get_global_id(0);
    a[i] = b[i];
}
