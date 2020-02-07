#version 410 core
in vec3 fragPos;
in vec3 normal;
out vec4 color;

void main(){
   float err = 0.35f-length(fragPos);
   err = err*10;
   color = vec4(err, err, err, 1.f);
   //scolor = vec4( clamp( dot(normalize(normal),normalize(vec3(0.0, 10.0, 2.5)-fragPos)), 0, 1 ) * vec3(1.0,1.0,1.0), 1.0);
}
