kernel void linear_move(global float* positions,
                        global float* velocities,
                        int vertex_count,
                        float multiplier)
{
    int index = get_global_id(0) * 3;

    float4 this_velocity = (float4)(velocities[index], velocities[index+1], velocities[index+2], 0);

    positions[index] += this_velocity.x * multiplier;
    positions[index+1] += this_velocity.y * multiplier;
    positions[index+2] += this_velocity.z * multiplier;
}
