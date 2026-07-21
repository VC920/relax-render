#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <cglm/cglm.h>

typedef struct {
    vec3 direction;
    vec3 color;
} DirectionalLight;

void directional_light_set_direction(DirectionalLight *light, float x, float y, float z);
void directional_light_set_color(DirectionalLight *light, float r, float g, float b);

#endif//DIRECTIONAL_LIGHT_H
