#version 410 core

in vec3 normal;
in vec3 fragPos;
out vec4 color;

uniform vec3 cameraPos;
vec3 lightPos = vec3(0.0, 10.0, 2.5);

vec3 specularColor= vec3(1, 1, 1);
vec3  diffuseColor = vec3(1., 0., 0.);
float shininessFactor = 0.5;
float attenIntensity = 1;
float ambientIntensity = 0.2;

void main() {

    vec3 lightDir = lightPos-fragPos;
    //blinn term
    float cosAngIncidence = dot(normal, lightDir);
    vec3 viewDirection = normalize(-cameraPos);
    vec3 halfAngle = normalize(lightDir + viewDirection);
    float blinnTerm = dot(normal, halfAngle);

    cosAngIncidence = clamp(cosAngIncidence, 0, 1);
    blinnTerm = clamp(blinnTerm, 0, 1);
    blinnTerm = cosAngIncidence != 0.0 ? blinnTerm : 0.0;
    blinnTerm = pow(blinnTerm, shininessFactor);

    vec3 outColor =
    (diffuseColor * attenIntensity * cosAngIncidence) +
    (specularColor * attenIntensity * blinnTerm) +
    (diffuseColor * ambientIntensity);

    color = vec4(outColor, 1.);


}
