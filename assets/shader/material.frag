#version 450 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 out_color;

in vec2 frag_texcoords;
in vec3 frag_pos;
in vec3 frag_normal;

uniform Material material;
uniform Light light;
uniform vec3 view_pos;

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, frag_texcoords));
    
    vec3 norm = normalize(frag_normal);
    vec3 light_dir = normalize(light.position - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, frag_texcoords));
    
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, frag_texcoords));
    
    out_color = vec4(ambient + diffuse + specular, 1.0);
}