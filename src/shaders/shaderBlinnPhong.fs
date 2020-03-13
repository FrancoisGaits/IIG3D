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

uniform vec3 _object_color;
uniform vec3 cameraPos;
uniform int _light_nb;

uniform Light _lights[N_MAX_LIGHT];

// Functions
vec3 calc_light(Light light, vec3 normal, vec3 frag_pos, vec3 view_dir);

void main(void) {
    vec3 norm = normalize(normal);
    vec3 view_dir = normalize(cameraPos - fragPosWorld);

    vec3 result = vec3(0,0,0);

    // Limits lights number
    int light_nb = _light_nb;
    if (_light_nb > N_MAX_LIGHT) {
        light_nb = N_MAX_LIGHT;
    }

    // Compute light impact
    for (int i = 0; i < light_nb; i++) {
        result += calc_light(_lights[i], norm, fragPosWorld, view_dir);
    }
//    Light key = Light(1, vec3(-1.5,0.5,1),vec3(1,1,1), vec3(0,0,-0.5), 0.88);
//    result = calc_point_light(key, norm, fragPosWorld, view_dir);
//
//    Light fill = Light(1, vec3(1.5,0.5,1),vec3(0.15,0.15,0.15), vec3(0,0,-0.5), 0.85);
//    result += calc_point_light(fill, norm, fragPosWorld, view_dir);
//
//    Light back = Light(1, vec3(0,1.5,-3),vec3(.65,.65,.65), vec3(0,0,-0.5), 0.8);
//    result += calc_point_light(back, norm, fragPosWorld, view_dir);

    //color = vec4(result * _object_color, 1.0);
    color = vec4(result, 1.0);

}

vec3 calc_light(Light light, vec3 normal, vec3 frag_pos, vec3 view_dir) {
    // ambient
    float ambient_amp = 0.1;
    float spotFactor = 1;

    if (light.type == 1) {
        spotFactor = dot(normalize(light.position - light.focus), normalize(light.position - frag_pos));
        ambient_amp = 0.0;
    }

    vec3 ambient = ambient_amp * light.color;
    if (light.type != 1 || spotFactor > light.limit) {

        // diffuse
        vec3 light_dir = normalize(light.position - frag_pos);
        float diff = max(dot(normal, light_dir), 0.0);
        vec3 diffuse = diff * light.color;

        // specular
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