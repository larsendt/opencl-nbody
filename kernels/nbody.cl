__kernel void nbody_move(__global float* positions, global float* velocities, int vertex_count)
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
		final_grav += normalize(direction) / (4000.0 * vertex_count);
	}

	velocities[index] += final_grav.x;
	velocities[index+1] += final_grav.y;
	velocities[index+2] += final_grav.z;

	positions[index] += velocities[index];
	positions[index+1] += velocities[index+1];
	positions[index+2] += velocities[index+2];
}
