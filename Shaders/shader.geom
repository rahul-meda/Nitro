
#version 330 core

layout (triangles) in;

layout (triangle_strip, max_vertices=256) out;

uniform int sub_divisions;
uniform mat4 PV;

void main()
{
	vec4 v0 = gl_in[0].gl_Position;
	vec4 v1 = gl_in[1].gl_Position;
	vec4 v2 = gl_in[2].gl_Position;

	float dx = abs(v0.x-v2.x)/sub_divisions;
	float dz = abs(v0.z-v1.z)/sub_divisions;

	float x = v0.x;
	float z = v0.z;

	for(int i = 0; i < sub_divisions*sub_divisions; i++)
	{
		// Check if the order of vertices matters
		gl_Position = PV * vec4(x, 0, z, 1);
		EmitVertex();
		gl_Position = PV * vec4(x+dx, 0, z, 1);
		EmitVertex();
		gl_Position = PV * vec4(x+dx, 0, z+dz, 1);
		EmitVertex();
		gl_Position = PV * vec4(x, 0, z+dz, 1);
		EmitVertex();
		EndPrimitive();

		x += dx;
		if((i+1)%sub_divisions == 0)
		{
			x = v0.x;
			z += dz;
		}
	}
}