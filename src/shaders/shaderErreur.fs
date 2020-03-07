#version 410 core
in vec3 fragPos;
in vec3 normal;

out vec4 color;

uniform int prec;
uniform float radius;


void main(){
   float err = radius-length(fragPos);
   err = err*8*prec;
   color = normalize(vec4(err+0.2/2, 1-err*err*2, 0.2, 1.f))*1.5;
}
