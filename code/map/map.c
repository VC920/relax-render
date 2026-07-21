#include "map.h"
#include <stdio.h>
#include <stb_ds.h>

Entity load_entity(char *line)
{
    char entity_type[256] = {0};
    char entity_name[256] = {0};
    char entity_model_path[256] = {0};
    char entity_diffuse_texture_path[256] = {0};
    char entity_specular_texture_path[256] = {0};
    float entity_position[3];
    float entity_rotation[3];
    float entity_scale;

    sscanf(line, "%s%s%s%s%s [%f/%f/%f] [%f/%f/%f] [%f]",
	   entity_type, entity_name, entity_model_path, entity_diffuse_texture_path, entity_specular_texture_path,
	   &entity_position[0], &entity_position[1], &entity_position[2],
	   &entity_rotation[0], &entity_rotation[1], &entity_rotation[2],
	   &entity_scale
	   );

    // Entity
    Entity entity;

    // Model
    Model model = model_load(entity_model_path);

    // Diffuse texture
    if (strcmp(entity_diffuse_texture_path, "NULL") != 0) {
	model.diffuse_texture = texture_load(entity_diffuse_texture_path);
    }

    // Specular texture
    if (strcmp(entity_specular_texture_path, "NULL") != 0) {
	model.specular_texture = texture_load(entity_specular_texture_path);
    }
	
    entity.model = model;	
    entity_set_position(&entity, entity_position[0], entity_position[1], entity_position[2]);
    entity_set_rotation(&entity, entity_rotation[0], entity_rotation[1], entity_rotation[2]);
    entity.scale = entity_scale;

    return entity;
}

DirectionalLight load_directional_light(char *line)
{
    char light_type[256] = {0};
    char light_name[256] = {0};
    float light_direction[3];
    float light_color[3];

    sscanf(line, "%s%s [%f/%f/%f] [%f/%f/%f]",
	   light_type, light_name,
	   &light_direction[0], &light_direction[1], &light_direction[2],
	   &light_color[0], &light_color[1], &light_color[2]
	   );

    // Entity
    DirectionalLight light;
    directional_light_set_direction(&light, light_direction[0], light_direction[1], light_direction[2]);
    directional_light_set_color(&light, light_color[0], light_color[1], light_color[2]);

    return light;
}

PointLight load_point_light(char *line)
{
    char light_type[256] = {0};
    char light_name[256] = {0};
    float light_position[3];
    float light_color[3];
    float light_constant;
    float light_linear;
    float light_quadratic;

    sscanf(line, "%s%s [%f/%f/%f] [%f/%f/%f] [%f] [%f] [%f]",
	   light_type, light_name,
	   &light_position[0], &light_position[1], &light_position[2],
	   &light_color[0], &light_color[1], &light_color[2],
	   &light_constant, &light_linear, &light_quadratic
	   );

    // Entity
    PointLight light;
    point_light_set_position(&light, light_position[0], light_position[1], light_position[2]);
    point_light_set_color(&light, light_color[0], light_color[1], light_color[2]);
    light.constant = light_constant;
    light.linear = light_linear;
    light.quadratic = light_quadratic;

    return light;
}

SpotLight load_spot_light(char *line)
{
    char light_type[256] = {0};
    char light_name[256] = {0};
    float light_position[3];
    float light_direction[3];
    float light_color[3];
    float light_constant;
    float light_linear;
    float light_quadratic;
    float light_cut_off;
    float light_outer_cut_off;

    sscanf(line, "%s%s [%f/%f/%f] [%f/%f/%f] [%f/%f/%f] [%f] [%f] [%f] [%f] [%f]",
	   light_type, light_name,
	   &light_position[0], &light_position[1], &light_position[2],
	   &light_direction[0], &light_direction[1], &light_direction[2],
	   &light_color[0], &light_color[1], &light_color[2],
	   &light_constant, &light_linear, &light_quadratic,
	   &light_cut_off, &light_outer_cut_off
	   );

    // Entity
    SpotLight light;
    spot_light_set_position(&light, light_position[0], light_position[1], light_position[2]);
    spot_light_set_direction(&light, light_direction[0], light_direction[1], light_direction[2]);
    spot_light_set_color(&light, light_color[0], light_color[1], light_color[2]);
    light.constant = light_constant;
    light.linear = light_linear;
    light.quadratic = light_quadratic;
    light.cut_off = light_cut_off;
    light.outer_cut_off = light_outer_cut_off;
    return light;
}

void map_load(const char *path, Entity **entities, DirectionalLight *directional_light, PointLight **point_lights, SpotLight **spot_lights)
{
    // Read file
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error not found %s\n", path);
    }

    char line[256]; // one line size
    while (fgets(line, sizeof(line), file) != NULL) {
	// Read type
	char type[256] = {0};
	sscanf(line, "%s", type);

	// Load static entity
	if (strcmp(type, "STATIC") == 0) {
	    Entity entity = load_entity(line);
	    arrpush(*entities, entity);
	}

	// Load directional light
	if (strcmp(type, "DIRECTIONAL_LIGHT") == 0) {
	    *directional_light = load_directional_light(line);
	}

	// Load point light
	if (strcmp(type, "POINT_LIGHT") == 0) {
	    PointLight point_light = load_point_light(line);
	    arrpush(*point_lights, point_light);
	}

	// Load spot light
	if (strcmp(type, "SPOT_LIGHT") == 0) {
	    SpotLight spot_light = load_spot_light(line);
	    arrpush(*spot_lights, spot_light);
	}
    }
    fclose(file);
}
