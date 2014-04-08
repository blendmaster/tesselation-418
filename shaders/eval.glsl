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

void main ()
{
	/* 
	Short names for positions of the vertices defining the patch.
	Note that we are still in the model coordinate system - no transformations
	were applied in the vertex or tesselation control shader.
	In your code, you may want to define pij for i,j in {0,1,2,3} in a 
	similar way. 
	*/
	vec3 p00 = gl_in[0].gl_Position.xyz;
	vec3 p01 = gl_in[1].gl_Position.xyz;
	vec3 p10 = gl_in[2].gl_Position.xyz;
	vec3 p11 = gl_in[3].gl_Position.xyz;

	/* 
	u and v are the parameter values provided by the tesselation primitive generator.
	Our goal is to compute the location of the vertex corresponding to these parameter
	values.
	*/
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	/* 
	... and here is the formula we use. p00 + u*(p10-p00) + v*(p01-p00) is the point
	on the parallelogram spanned by p00->p10 and p00->p01 parameterized using these
	two vectors. We add a funny perturbation on top of that.
	In your code, you should use one of the forms of the bi-cubic Bezier formula
	to compute the vertex coordinates corresponding to (u,v) from the 16 control points.
	*/
	coords = p00 + u*(p10-p00) + v*(p01-p00);
}
