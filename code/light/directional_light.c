#include "directional_light.h"

void directional_light_set_direction(DirectionalLight *light, float x, float y, float z)
{
    light->direction[0] = x;
    light->direction[1] = y;
    light->direction[2] = z;
}

void directional_light_set_color(DirectionalLight *light, float r, float g, float b)
{
    light->color[0] = r;
    light->color[1] = g;
    light->color[2] = b;
}
