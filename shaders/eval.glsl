#version 400

/* TESSELATION EVALUATION SHADER */

/* 
This means that the parameter domain for our patches is a square [0,1]^2.
Keep it like this for bi-cubic Bezier patches. See 
http://www.opengl.org/wiki/Tessellation_Evaluation_Shader
for more options.
*/
layout(quads) in;

/* 
we'll output the model coordinates of every vertex on the tesselated patch...
*/
out vec3 coords;

/*
 * matrix form of bicubic bezier curve
 */
const mat4 M = mat4(1, -3, 3, -1,
		             0, 3, -6, 3,
		             0, 0, 3, -3,
		             0, 0, 0, 1);
const mat4 MT = transpose(
		mat4(1, -3, 3, -1,
			 0, 3, -6, 3,
			 0, 0, 3, -3,
			 0, 0, 0, 1));

void main ()
{	/*
	Short names for positions of the vertices defining the patch.
	Note that we are still in the model coordinate system - no transformations
	were applied in the vertex or tesselation control shader.
	*/
	vec3 p00 = gl_in[0].gl_Position.xyz;
	vec3 p01 = gl_in[1].gl_Position.xyz;
	vec3 p02 = gl_in[2].gl_Position.xyz;
	vec3 p03 = gl_in[3].gl_Position.xyz;
	vec3 p10 = gl_in[4].gl_Position.xyz;
	vec3 p11 = gl_in[5].gl_Position.xyz;
	vec3 p12 = gl_in[6].gl_Position.xyz;
	vec3 p13 = gl_in[7].gl_Position.xyz;
	vec3 p20 = gl_in[8].gl_Position.xyz;
	vec3 p21 = gl_in[9].gl_Position.xyz;
	vec3 p22 = gl_in[10].gl_Position.xyz;
	vec3 p23 = gl_in[11].gl_Position.xyz;
	vec3 p30 = gl_in[12].gl_Position.xyz;
	vec3 p31 = gl_in[13].gl_Position.xyz;
	vec3 p32 = gl_in[14].gl_Position.xyz;
	vec3 p33 = gl_in[15].gl_Position.xyz;

	// column major order
	// TODO apparently mat4 of vertices is not possible, might need to do Px, Py, Pz and so on
	mat4 P = mat4(
      vec4(p00, p10, p20, p30),
      vec4(p01, p11, p21, p31),
      vec4(p02, p12, p22, p32),
      vec4(p03, p13, p23, p33)
	);

	/* 
	u and v are the parameter values provided by the tesselation primitive generator.
	Our goal is to compute the location of the vertex corresponding to these parameter
	values.
	*/
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	vec4 us = vec4(1, u, u*u, u*u*u);
	vec4 vs = vec4(1, v, v*v, v*v*v);

	/* use one of the forms of the bi-cubic Bezier formula
	to compute the vertex coordinates corresponding to (u,v) from the 16 control points.
	*/
	coords = (us * M * P * MT * vs).xyz;
}
