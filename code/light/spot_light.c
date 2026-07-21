#include "spot_light.h"

void spot_light_set_position(SpotLight *light, float x, float y, float z)
{
    light->position[0] = x;
    light->position[1] = y;
    light->position[2] = z;
}

void spot_light_set_direction(SpotLight *light, float x, float y, float z)
{
    light->direction[0] = x;
    light->direction[1] = y;
    light->direction[2] = z;
}

void spot_light_set_color(SpotLight *light, float r, float g, float b)
{
    light->color[0] = r;
    light->color[1] = g;
    light->color[2] = b;
}
