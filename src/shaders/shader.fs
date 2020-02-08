#version 410 core
in vec3 fragPos;
in vec3 normal;
out vec4 color;

void main(){
   /*float err = 0.35f-length(fragPos);
   err = err*10;
   color = vec4(err, err, err, 1.f);*/


   //color = vec4( clamp( dot(normalize(normal),normalize(vec3(0.0, 10.0, 2.5)-fragPos)), 0, 1 ) * vec3(1.0,1.0,1.0), 1.0);

   vec4 color1 = vec4( clamp( dot(normalize(cross(dFdx(fragPos),dFdy(fragPos))),normalize(vec3(0.0, 10.0, 2.5)-fragPos)), 0, 1 ) * vec3(0.5f,1.f,1.f), 1.0);
   vec4 color2 = vec4( clamp( dot(normalize(cross(dFdx(fragPos),dFdy(fragPos))),normalize(vec3(0.0, -5.0, -12.5)-fragPos)), 0, 1 ) * vec3(0.8f,0.5f,1.f), 1.0);
   vec4 color3 = vec4( clamp( dot(normalize(cross(dFdx(fragPos),dFdy(fragPos))),normalize(vec3(0.0, -10.0, 2.5)-fragPos)), 0, 1 ) * vec3(0.8f,0.8f,0.8f), 1.0);

   color = color1 + color2 + color3;
}
