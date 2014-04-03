#version 400

/*
This vertex shader just passes the vertex locations in model
coordinate system to the next stage (tesselation control shader)
*/

layout(location=0) in vec3 coord;  

void main() {  
  gl_Position = vec4(coord,1.0);           
} 
