#version 450 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;

    float constant;
    float linear;
    float quadratic;

    float cut_off;
    float outer_cut_off;
};

out vec4 out_color;	

in vec2 frag_texcoords;
in vec3 frag_pos;
in vec3 frag_normal;

#define AMBIENT_STRENGTH 0.1
uniform Material material;

uniform vec3 view_pos;

uniform DirLight dir_light;
#define NR_POINT_LIGHTS 3
uniform PointLight point_lights[NR_POINT_LIGHTS];
#define NR_SPOT_LIGHTS 3
uniform SpotLight spot_lights[NR_SPOT_LIGHTS];

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(-light.direction);
    
    // Diffuse
    float diff = max(dot(normal, light_dir), 0.0);
    
    // Specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    
    // Result
    vec3 diffuse = light.color * diff * vec3(texture(material.diffuse, frag_texcoords));
    vec3 specular = light.color * spec * vec3(texture(material.specular, frag_texcoords));
    return (diffuse + specular);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position - frag_pos);

    // Diffuse
    float diff = max(dot(normal, light_dir), 0.0);

    // Specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Result
    vec3 diffuse = light.color * diff * vec3(texture(material.diffuse, frag_texcoords));
    vec3 specular = light.color * spec * vec3(texture(material.specular, frag_texcoords));
    diffuse *= attenuation;
    specular *= attenuation;
    return (diffuse + specular);
}

vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position - frag_pos);

    // Diffuse
    float diff = max(dot(normal, light_dir), 0.0);

    // Specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Intensity
    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.cut_off - light.outer_cut_off;
    float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

    // Result
    vec3 diffuse = light.color * diff * vec3(texture(material.diffuse, frag_texcoords));
    vec3 specular = light.color * spec * vec3(texture(material.specular, frag_texcoords));
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (diffuse + specular);
}

void main()
{
    vec3 norm = normalize(frag_normal);
    vec3 view_dir = normalize(view_pos - frag_pos);

    // Ambient light
    vec3 ambient = AMBIENT_STRENGTH * vec3(texture(material.diffuse, frag_texcoords));
    vec3 result = ambient;

    // Directional light
    result += calc_dir_light(dir_light, norm, view_dir);

    // Point Light
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calc_point_light(point_lights[i], norm, frag_pos, view_dir);
    }

    // Spot Light
    for (int i = 0; i < NR_SPOT_LIGHTS; i++) {
        result += calc_spot_light(spot_lights[i], norm, frag_pos, view_dir);
    }

    // Result
    out_color = vec4(result, 1.0);
}