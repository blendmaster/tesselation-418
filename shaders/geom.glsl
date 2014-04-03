#version 400
#extension GL_EXT_geometry_shader4 : enable

/* 
In our case, the tesselation stage outputs triangles with vertices
output by the tesselation evaluation shader
*/
layout (triangles) in;

/* 
We'll generate 6 triangles from a single input triangle (6*3=18).
Basically, there is now just one patch, corresponding to one of the faces
of the cube; we'll use 6-way cube symmetry to generate patches for all faces.
*/
layout (triangle_strip, max_vertices=18) out;

/* 
coords is arriving here from the tesselation shader
*/
in vec3 coords[3];

/*
we'll evaluate the illumination formula here. This is the output sent to the 
fragment shader. We'll do flat shading.
*/
flat out vec3 color;

/* 
... and transform from model to world coordinate system
*/
uniform mat4 P;
uniform mat4 MV; 

/*
Usual variables needed for illumination. AI = I_a * k_a.
We'll do greyscale rendering here, so the K's are scalars.
*/
uniform float I;
uniform float AI; 
uniform vec3 lloc;
uniform float Kd, Ks;
uniform float nspec;

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

	for ( int s=-1; s<=1; s+=2 )
		for ( int j=0; j<3; j++ ) {
			for ( int i=0; i<3; i++ ) {

				vec3 permuted_coords, permuted_normal;
				vec3 normal = cross(coords[1]-coords[0],coords[2]-coords[0]);

				/* swap and perhaps negate the coordinates as described above... */

				for ( int k=0; k<3; k++ ) {
					permuted_coords[k] = s*coords[i][(k+j)%3];
					permuted_normal[k] = s*normal[(k+j)%3];
				}

				/* transform to the world coordinate system */
				normal = mat3(MV)*permuted_normal;
				permuted_coords = (MV*vec4(permuted_coords,1.0)).xyz;

				/* evaluate the illumination formula as usual */
				vec3 N = normalize(normal);
				vec3 L = normalize(lloc-permuted_coords);
				vec3 V = normalize(-permuted_coords);
				vec3 H = normalize(L+V);
  				float NdotL = dot(N,L);
				float HdotN = dot(N,H);
				if (HdotN<0) HdotN = 0;
				if (NdotL<0) NdotL = 0;
				color = vec3(I*(Kd*NdotL + Ks*pow(HdotN,nspec)) + AI);  

				/* apply projection matrix and emit the vertex */
				gl_Position = P*vec4(permuted_coords,1);
				EmitVertex();
			}
			EndPrimitive();
		}
}
