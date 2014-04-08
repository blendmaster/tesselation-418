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
The normal at the vertex for phong shading
and a flag for reflection in geometry shader
*/
out vec3 coords;
out vec3 normals;
out int needsLeftRightReflection;

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

vec3 coordAt(mat4 Px, mat4 Py, mat4 Pz, float u, float v) {
	vec4 us = vec4(1, u, u*u, u*u*u);
	vec4 vs = vec4(1, v, v*v, v*v*v);

	/* use one of the forms of the bi-cubic Bezier formula
	to compute the vertex coordinates corresponding to (u,v) from the 16 control points.
	In matrix math, this is
	[1, u, u*u, u*u*u] * M * P * MT * [1; v; v*v; v*v*V]
	*/
	return vec3(
		dot(((us * M) * Px) * MT, vs),
		dot(((us * M) * Py) * MT, vs),
		dot(((us * M) * Pz) * MT, vs)
	);
}

vec3 normalAt(mat4 Px, mat4 Py, mat4 Pz, float u, float v) {
	vec3 point = coordAt(Px, Py, Pz, u, v);
	vec3 pu = coordAt(Px, Py, Pz, u + 0.01, v       );
	vec3 pv = coordAt(Px, Py, Pz, u       , v + 0.01);

	vec3 du = pu - point;
	vec3 dv = pv - point;

	return normalize(cross(dv, du));
}

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

	mat4 Px = mat4(
      vec4(p00.x, p10.x, p20.x, p30.x),
      vec4(p01.x, p11.x, p21.x, p31.x),
      vec4(p02.x, p12.x, p22.x, p32.x),
      vec4(p03.x, p13.x, p23.x, p33.x)
	);

	mat4 Py = mat4(
	  vec4(p00.y, p10.y, p20.y, p30.y),
	  vec4(p01.y, p11.y, p21.y, p31.y),
	  vec4(p02.y, p12.y, p22.y, p32.y),
	  vec4(p03.y, p13.y, p23.y, p33.y)
	);

	mat4 Pz = mat4(
	  vec4(p00.z, p10.z, p20.z, p30.z),
	  vec4(p01.z, p11.z, p21.z, p31.z),
	  vec4(p02.z, p12.z, p22.z, p32.z),
	  vec4(p03.z, p13.z, p23.z, p33.z)
	);

	/* 
	u and v are the parameter values provided by the tesselation primitive generator.
	Our goal is to compute the location of the vertex corresponding to these parameter
	values.
	*/
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	coords = coordAt(Px, Py, Pz, u, v);
	normals = normalAt(Px, Py, Pz, u, v);

	needsLeftRightReflection = gl_PrimitiveID < 4 ? 0 : 1;
}
