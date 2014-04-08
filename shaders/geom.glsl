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
	/* 
	From one face of a cube, how to get all the other ones?
	We apply transformations 
	(x,y,z) -> (sx,sy,sz) (s=+-1 - this is for j=0)
	(x,y,z) -> (sy,sz,sx) (j=1)
	(x,y,z) -> (sz,sx,sy) (j=2)
	to generate all related by 6-way cube symmetry.
	*/

	for ( int i=0; i<3; i++ ) {
		vec3 permuted_coords, permuted_normal;

		// TODO for phong, need to somehow get normals from adjacent
		// vertices in order to smoothly interpolate. Right now, each vertex
		// is basically isolated, so phong shading results in flat shading.
		vec3 normal = cross(coords[1]-coords[0],coords[2]-coords[0]);

		/* swap and perhaps negate the coordinates as described above... */

		for ( int k=0; k<3; k++ ) {
			permuted_coords[k] = -1*coords[i][k];
			permuted_normal[k] = -1*normal[k];
		}

		/* transform to the world coordinate system */
		normal = mat3(MV)*permuted_normal;
		permuted_coords = (MV*vec4(permuted_coords,1.0)).xyz;

		/* pass phong output params, which are then interpolated */
		_wnormal = normalize(normal);
		_wcoord = permuted_coords;

		/* apply projection matrix and emit the vertex */
		gl_Position = P*vec4(permuted_coords,1);
		EmitVertex();
	}
	EndPrimitive();
}
