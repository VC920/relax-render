#ifndef SPOT_LIGHT
#define SPOT_LIGHT

#include <cglm/cglm.h>

typedef struct {
    vec3 position;
    vec3 color;
    
    float constant;
    float linear;
    float quadratic;
} PointLight;

void point_light_set_position(PointLight *light, float x, float y, float z);
void point_light_set_color(PointLight *light, float r, float g, float b);

#endif//SPOT_LIGHT
