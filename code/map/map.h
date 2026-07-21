#ifndef MAP_H
#define MAP_H

#include "entity.h"
#include "directional_light.h"
#include "point_light.h"
#include "spot_light.h"

void map_load(const char *path, Entity **entities, DirectionalLight *directional_light, PointLight **point_lights, SpotLight **spot_lights);

#endif//MAP_H
