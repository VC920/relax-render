#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <cglm/cglm.h>

typedef struct {
    vec3 position;
    vec3 direction;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
    
    float cut_off;
    float outer_cut_off;
} SpotLight;

void spot_light_set_position(SpotLight *light, float x, float y, float z);
void spot_light_set_direction(SpotLight *light, float x, float y, float z);
void spot_light_set_color(SpotLight *light, float r, float g, float b);

#endif//SPOT_LIGHT_H
