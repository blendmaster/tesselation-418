#version 400
 
/* 
Use color (illumination formula is evaluated in the 
geometry shader) as the final fragment color 
*/

flat in vec3 color;  

out vec3 fragcolor;
 
void main() { 
	fragcolor = color; 
}

