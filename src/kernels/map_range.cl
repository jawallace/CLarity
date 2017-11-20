//! @file       map_range.cl
//! @brief      Defines an OpenCL kernel to perform range mapping via ray-casting
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

//! @brief  Compute the range at each pixel in the image.
//!
//! @param[in]  origin          the location of the camera, in world coordinates
//! @param[in]  world_coords    the pointing vector of each pixel, in world coordinats
//! @param[in]  height_map      the terrain height map
//! @param[in]  scale           the scale of the terrain map, in meters-per-pixel
//! @param[in]  max_range       the maximum range allowed, in meters
//! @param[in]  max_error       the maximum error of the image, in meters
//! @param[in]  bounds          the bounds of the heightmap, pixels
//! @param[in]  pitch           the pitch of the image
//! @param[out] range           the output buffer of range-per-pixel
__kernel void map_range(const float3 origin,
                        constant float3 * world_coords,
                        constant float * height_map,
                        const float scale,
                        const float max_range,
                        const float max_error,
                        const float2 bounds,
                        const int pitch,
                        __global float * range)
{

    // Get location and corresponding input values
    const int2 pos = { get_global_id(0), get_global_id(1) };
    const int offset = pos.x * pitch + pos.y;
    const float3 pv = world_coords[offset];
   
    // Determine parameters of the walk
    float step = max_error / scale;
    const int iterations = ceil(max_range / max_error);

    // Perform the walk
    float3 loc = origin / scale;

    for (int i = 0; i < iterations; i++) {
        loc = loc + (step * pv);

        const int r = clamp(loc.x, 0.0f, bounds.y);
        const int c = clamp(bounds.x - loc.y, 0.0f, bounds.x);

        const float height = height_map[r * pitch + c];
        const int keep_going = (height >= loc.z);

        // setup the next step
        // If we have hit the ground, keep_going will be 0, and we won't move forward anymore
        // Otherwise, it will be 1 and step will remain the same for the next iteration
        step = step * keep_going;
    }

    // After all iterations, the range is the length of the vector difference of our current
    // location and the origin
    const float range_pixels = length(loc - origin);

    range[offset] = clamp(scale * range_pixels, 0.0f, max_range);
}

