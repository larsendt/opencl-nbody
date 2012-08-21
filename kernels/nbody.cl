kernel void nbody_move(global float* positions, 
                       global float* velocities, 
                       int vertex_count, 
                       float4 external_force_pos,
                       float external_force_strength)
{
	int index = get_global_id(0) * 3;
	
	float4 final_grav = (float4)(0, 0, 0, 0);
	float4 this_vertex = (float4)(positions[index], positions[index+1], positions[index+2], 0);
	float dist2;
	
	for(int i = 0; i < vertex_count; i++)
	{
		int vi = i*3;
		float4 other_vertex = (float4)(positions[vi], positions[vi+1], positions[vi+2], 0);
		float4 direction = other_vertex - this_vertex;
        float dist = length(direction);
		final_grav += normalize(direction) / ((pow(dist, 2) + 0.01) * 5000000);//(4000.0 * vertex_count);
	}

    float4 external_force_direction = this_vertex - external_force_pos;
    float dist = length(external_force_direction);
    final_grav += normalize(external_force_direction) * external_force_strength / (pow(dist, 2) + 0.001);

	velocities[index] += final_grav.x;
	velocities[index+1] += final_grav.y;
	velocities[index+2] += final_grav.z;

	positions[index] += velocities[index];
	positions[index+1] += velocities[index+1];
	positions[index+2] += velocities[index+2];

    const float bounds = 1.0;
    if(positions[index] > bounds || positions[index] < -bounds)
    {
        velocities[index] *= -0.9;
    }
    
    if(positions[index+1] > bounds || positions[index+1] < -bounds)
    {
        velocities[index+1] *= -0.9;
    }

    if(positions[index+2] > bounds || positions[index+2] < -bounds)
    {
        velocities[index+2] *= -0.9;
    }
}
