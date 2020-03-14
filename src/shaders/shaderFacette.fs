#version 410 core
in vec3 fragPos;
out vec4 color;

void main(){
    vec4 color1 = vec4(clamp(dot(normalize(cross(dFdx(fragPos), dFdy(fragPos))), normalize(vec3(0.0, 10.0, 2.5)-fragPos)), 0, 1) * vec3(0.5f, 1.f, 1.f), 1.0);
    vec4 color2 = vec4(clamp(dot(normalize(cross(dFdx(fragPos), dFdy(fragPos))), normalize(vec3(0.0, -5.0, -12.5)-fragPos)), 0, 1) * vec3(0.8f, 0.5f, 1.f), 1.0);
    vec4 color3 = vec4(clamp(dot(normalize(cross(dFdx(fragPos), dFdy(fragPos))), normalize(vec3(0.0, -10.0, 2.5)-fragPos)), 0, 1) * vec3(0.3f, 0.8f, 0.1f), 1.0);

    color = color1 + color2 + color3;
}
