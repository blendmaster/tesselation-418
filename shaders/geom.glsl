#version 400
#extension GL_EXT_geometry_shader4 : enable

/* 
In our case, the tesselation stage outputs triangles with vertices
output by the tesselation evaluation shader
*/
layout (triangles) in;

/* 
We'll generate up to 4 triangles from a single input triangle.
*/
layout (triangle_strip, max_vertices=12) out;

/* 
coords is arriving here from the tesselation shader
needsLeftRightReflection is a flag for disambiguating the handle and spout
*/
in vec3 coords[3];
in int[3] needsLeftRightReflection;

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

	// generate reflection around the xz plane (back and front);
	for (int k = -1; k <= 1; k += 2) {

		// generate reflection around the xy plane (left and right), unless
		// we're generating the handle or the spout.
		int j = needsLeftRightReflection[0] == 1 ? -1 : 1;
		for (; j <= 1; j += 2) {
			for (int i=0; i<3; i++ ) {
				vec3 coord = coords[i];

				coord.x *= j;
				coord.z *= k;

				/* pass phong output params, which are then interpolated */
				_wnormal = normal;
				_wcoord = (MV*vec4(coord,1.0)).xyz;

				/* apply projection matrix and emit the vertex */
				gl_Position = P*vec4(_wcoord,1);
				EmitVertex();
			}
			EndPrimitive();
		}
	}
}
