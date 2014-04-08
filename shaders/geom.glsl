#version 400
#extension GL_EXT_geometry_shader4 : enable

/* 
In our case, the tesselation stage outputs triangles with vertices
output by the tesselation evaluation shader
*/
layout (triangles) in;

/* 
We'll generate 1 triangle from a single input triangle.
*/
layout (triangle_strip, max_vertices=3) out;

/* 
coords is arriving here from the tesselation shader
*/
in vec3 coords[3];

/*
 * vanilla phong shading out
 */
noperspective out vec3 _wcoord;
noperspective out vec3 _wnormal;

/* 
... and transform from model to world coordinate system
*/
uniform mat4 P;
uniform mat4 MV; 

void main(void)
{
	// TODO for phong, need to somehow get normals from adjacent
	// vertices in order to smoothly interpolate. Right now, each vertex
	// is basically isolated, so phong shading results in flat shading.
	vec3 normal = normalize(mat3(MV) * cross(coords[1]-coords[0],coords[2]-coords[0]));

	for ( int i=0; i<3; i++ ) {
		/* pass phong output params, which are then interpolated */
		_wnormal = normal;
		_wcoord = (MV*vec4(coords[i],1.0)).xyz;

		/* apply projection matrix and emit the vertex */
		gl_Position = P*vec4(_wcoord,1);
		EmitVertex();
	}
	EndPrimitive();
}
