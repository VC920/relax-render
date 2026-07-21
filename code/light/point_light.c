#include "point_light.h"

void point_light_set_position(PointLight *light, float x, float y, float z)
{
    light->position[0] = x;
    light->position[1] = y;
    light->position[2] = z;
}

void point_light_set_color(PointLight *light, float r, float g, float b)
{
    light->color[0] = r;
    light->color[1] = g;
    light->color[2] = b;
}
