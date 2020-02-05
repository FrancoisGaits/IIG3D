#version 410 core
in vec3 fragPos;
out vec4 color;

void main(){
   color = vec4( clamp( dot(normalize(cross(dFdx(fragPos),dFdy(fragPos))),normalize(vec3(0.0, 10.0, 2.5)-fragPos)), 0, 1 ) * vec3(1.0,1.0,1.0), 1.0);
}
