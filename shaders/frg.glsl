#version 400
 
/* 
vanilla phong shading.
*/

noperspective in vec3 _wnormal;
noperspective in vec3 _wcoord;

out vec3 fragcolor;

/*
Usual variables needed for illumination. AI = I_a * k_a.
We'll do greyscale rendering here, so the K's are scalars.
*/
uniform float I;
uniform float AI;
uniform vec3 lloc;
uniform float Kd, Ks;
uniform float nspec;
 
void main() { 
    vec3 N = normalize(_wnormal);
    vec3 L = normalize(lloc-_wcoord);
    vec3 V = -normalize(_wcoord);
    vec3 H = normalize(L+V);
	float NdotL = dot(N,L);
	float HdotN = dot(N,H);
	if (HdotN<0) HdotN = 0;
	if (NdotL<0) NdotL = 0;

    fragcolor = vec3(I*(Kd*NdotL + Ks*pow(HdotN,nspec))) + AI;
}

