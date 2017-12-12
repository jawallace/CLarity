//! @file       cam_2_world_coords.cl
//! @brief      Defines an OpenCL kernel to compute world coordinates for each pixel.
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

//! @brief  Compute the pointing vector of each pixel in the camera in world coordinates
//!
//! @param[in]  cam_coords      the camera coordinates of each pixel
//! @param[in]  rotation        the rotation matrix for the Camera's orientation
//! @param[in]  pitch           the pitch of the image
//! @param[out] world_coords    the output buffer of world coordinates
__kernel void cam2world(__global float4 * cam_coords, 
                        __global float4 * rot_mat,
                        const int pitch,
                        __global float4 * world_coords)
{
    // Get location and corresponding camera coordinate
    const int2 pos = { get_global_id(0), get_global_id(1) };
    const int offset = pos.x * pitch + pos.y;
    const float4 cam_coord = cam_coords[offset];

    float4 world_coord = { dot(rot_mat[0].xyz, cam_coord.xyz), 
                           dot(rot_mat[1].xyz, cam_coord.xyz), 
                           dot(rot_mat[2].xyz, cam_coord.xyz),
                           0.0 };

    world_coord.xyz = world_coord.xyz / length(world_coord.xyz);

    // Write out
    world_coords[offset] = world_coord;
}

