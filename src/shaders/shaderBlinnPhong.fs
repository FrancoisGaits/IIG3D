#version 410 core

#define N_MAX_LIGHT 32

struct Light {
    int type;
    vec3 position;
    vec3 color;
    vec3 focus;
    float limit;
};

in vec3 normal;
in vec3 fragPosWorld;
out vec4 color;

uniform vec3 objectColor;
uniform vec3 cameraPos;
uniform int nbLight;

uniform Light lights[N_MAX_LIGHT];

// Functions
vec3 calc_light(Light light, vec3 normal, vec3 frag_pos, vec3 view_dir);

void main(void) {
    vec3 norm = normalize(normal);
    vec3 view_dir = normalize(cameraPos - fragPosWorld);

    vec3 result = vec3(0,0,0);

    // Limits lights number
    int light_nb = nbLight;
    if (nbLight > N_MAX_LIGHT) {
        light_nb = N_MAX_LIGHT;
    }

    // Compute light impact
    for (int i = 0; i < light_nb; i++) {
        result += calc_light(lights[i], norm, fragPosWorld, view_dir);
    }

    color = vec4(result * objectColor, 1.0);
}

vec3 calc_light(Light light, vec3 normal, vec3 frag_pos, vec3 view_dir) {

    float ambient_amp = 0.1;
    float spotFactor = 1;

    if (light.type == 1) {
        spotFactor = dot(normalize(light.position - light.focus), normalize(light.position - frag_pos));
        ambient_amp = 0.0;
    }

    vec3 ambient = ambient_amp * light.color;
    if (light.type != 1 || spotFactor > light.limit) {

        vec3 light_dir = normalize(light.position - frag_pos);
        float diff = max(dot(normal, light_dir), 0.0);
        vec3 diffuse = diff * light.color;

        float specular_strength = 0.5;
        vec3 reflect_dir = reflect(-light_dir, normal);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 16);
        vec3 specular = specular_strength * spec * light.color;

        return (ambient + diffuse + specular) * ((1.0 - (1.0 - spotFactor) * 1.0/(1.0 - light.limit))) ;
    }
    else {
        return vec3(0);
    }
}