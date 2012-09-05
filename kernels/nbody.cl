kernel void nbody_move(global float* positions, 
                       global float* velocities, 
                       global float* masses,
                       int vertex_count,
                       float multiplier)
{
	int index = get_global_id(0) * 3;
	
	float4 total_force = (float4)(0, 0, 0, 0);
    float4 this_acceleration;
	float4 this_vertex = (float4)(positions[index], positions[index+1], positions[index+2], 0);
    float this_mass = masses[get_global_id(0)];
	float dist2;
	
	for(int i = 0; i < vertex_count; i++)
	{
		int vi = i*3;
		float4 other_vertex = (float4)(positions[vi], positions[vi+1], positions[vi+2], 0);
        float other_mass = masses[i];
		float4 direction = other_vertex - this_vertex;
        float dist = length(direction);
		total_force += normalize(direction) * other_mass / (vertex_count * 10000.0);
	}

    this_acceleration = total_force / this_mass;
    this_acceleration *= multiplier;

	velocities[index] += this_acceleration.x;
	velocities[index+1] += this_acceleration.y;
	velocities[index+2] += this_acceleration.z;

	positions[index] += velocities[index] * multiplier;
	positions[index+1] += velocities[index+1] * multiplier;
	positions[index+2] += velocities[index+2] * multiplier;

/*    const float bounds = 5.0;
    if(positions[index] > bounds)
    {
        velocities[index] *= -0.5;
        positions[index] = bounds; 
    }
    else if(positions[index] < -bounds)
    {
        velocities[index] *= -0.5;
        positions[index] = -bounds;
    }
    
    if(positions[index+1] > bounds)
    {
        velocities[index+1] *= -0.5;
        positions[index+1] = bounds;
    }
    else if(positions[index+1] < -bounds)
    {
        velocities[index+1] *= -0.5;
        positions[index+1] = -bounds;
    }

    if(positions[index+2] > bounds)
    {
        velocities[index+2] *= -0.5;
        positions[index+2] = bounds;
    }
    else if(positions[index+2] < -bounds)
    {
        velocities[index+2] *= -0.5;
        positions[index+2] = -bounds;
    }
*/
}
