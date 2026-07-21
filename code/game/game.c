#include "game.h"
#include <stdio.h>
#include <stdbool.h>
#include <stb_ds.h>
#include <cglm/cglm.h>
#include <SDL2/SDL.h>
#include "entity.h"
#include "renderer.h"
#include "input.h"
#include "map.h"
#include "camera.h"
#include "directional_light.h"
#include "point_light.h"
#include "spot_light.h"

typedef enum {
    NORMAL,
    EDITOR
} game_state;

static Shader material_shader;
static Shader base_shader;
static Entity *static_entities = NULL;
static DirectionalLight direction_light;
static PointLight *point_lights = NULL;
static SpotLight *spot_lights = NULL;
static Camera camera;
static game_state main_state; 

void game_init()
{
    // Init renderer
    material_shader = shader_load("assets/shader/material.vert", "assets/shader/material.frag");
    base_shader = shader_load("assets/shader/base.vert", "assets/shader/base.frag");
    renderer_init();

    // Init mode
    main_state = NORMAL;

    // Init entities
    // entities = map_load_entities("assets/map/light_test.map");
    map_load("assets/map/main.map", &static_entities, &direction_light, &point_lights, &spot_lights);

    // Init camera
    vec3 cam_pos = { 0.0f, 1.0f, 10.0f };
    camera = camera_init(cam_pos, 5.0f, 0.05f);
}

void game_update(float dt)
{
    // printf("Current FPS: %.f\n", 1 / delta_time);

    // Check state
    if (input_key_down('1')) main_state = NORMAL;
    if (input_key_down('2')) main_state = EDITOR;
    
    if (main_state == EDITOR) {
	camera.can_fly = true;
    } else {
	camera.can_fly = false;
    }

    // Update camera    
    camera.move_front = input_key_down('w');
    camera.move_back  = input_key_down('s');
    camera.move_left  = input_key_down('a');
    camera.move_right = input_key_down('d');
    camera.move_up    = input_key_down('q');
    camera.move_down  = input_key_down('e');

    int mouse_xrel = 0;
    int mouse_yrel = 0;
    input_mouse_rel_pos(&mouse_xrel, &mouse_yrel);

    camera_rotate(&camera, mouse_xrel, mouse_yrel);
    camera_move(&camera, dt);

    input_reset_mouse_rel_pos();
}

void game_render()
{
    renderer_clear();

    // Render entity
    for (int i = 0; i < arrlen(static_entities); i++) {
        renderer_render(&material_shader, &direction_light, point_lights, spot_lights, &static_entities[i], &camera);
    }

    // Render light
    for (int i = 0; i < arrlen(point_lights); i++) {
	renderer_render_sphere(&base_shader, point_lights[i].position, point_lights[i].color, &camera);
    }
    for (int i = 0; i < arrlen(spot_lights); i++) {
	renderer_render_sphere(&base_shader, spot_lights[i].position, spot_lights[i].color, &camera);
    }
}
