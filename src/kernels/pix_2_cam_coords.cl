//! @file       pix_2_cam_coords.cl
//! @brief      Defines an OpenCL kernel to compute camera coordinates for each pixel
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

//! @brief  Compute the pointing vector of each pixel in the camera, relative to the camera
//!         boresight.
//!
//! @detail Reference: Francesco Callari https://stackoverflow.com/a/32530762
//!
//! @param[in]  boresight       the boresight vector
//! @param[in]  pitch           the pitch of the image
//! @param[out] cam_coords      the output buffer of camera coordinates
__kernel void pix2cam(const float3 boresight, 
                      const int pitch,
                      __global float3 * cam_coords)
{
    // Get location
    const float2 pos = { get_global_id(0), get_global_id(1) };

    // Get pixel vector relative to boresight
    float2 pixel2d = pos - boresight.xy;
    const float3 pixel = { pixel2d.x, pixel2d.y, boresight.z };

    // compute angle between pixel and boresight
    const float ang = acos(dot(boresight, pixel)) / (length(pixel) * length(boresight));

    // Image plane angle
    const float phi = atan2(pixel.x, pixel.y);

    // Compute camera coordinates
    float3 new_coord = { cos(ang), sin(ang) * cos(phi), sin(ang) * sin(phi) };

    // Write out
    cam_coords[get_global_id(0) * pitch + get_global_id(1)] = new_coord;
}
