#ifndef RENDERER_H
#define RENDERER_H

#include <cglm/cglm.h>

#include "shader.h"
#include "entity.h"
#include "camera.h"
#include "directional_light.h"
#include "point_light.h"
#include "spot_light.h"

void renderer_init();
void renderer_clear();
void renderer_render(Shader *shader, DirectionalLight *dir_light, PointLight *point_lights, SpotLight *spot_lights,
		     Entity *entity, Camera *camera);
void renderer_render_sphere(Shader *shader, vec3 position, vec3 color, Camera *camera);

#endif//RENDERER_H
