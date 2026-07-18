#include "game.h"
#include <stdio.h>
#include <stdbool.h>
#include <stb_ds.h>
#include <cglm/cglm.h>
#include "entity.h"
#include "renderer.h"
#include "input.h"
#include "map.h"
#include "camera.h"
#include "light.h"

typedef enum {
    NORMAL,
    EDITOR
} game_state;

static Shader entity_shader;
static Shader light_shader;
static Entity *entities;
static Light light;
static Entity light_entity;
static Camera camera;
static game_state main_state; 

void game_init()
{
    // Init renderer
    entity_shader = shader_load("assets/shader/material.vert", "assets/shader/material.frag");
    light_shader = shader_load("assets/shader/material.vert", "assets/shader/sun.frag");
    renderer_init();

    // Init mode
    main_state = NORMAL;

    // Init entities
    // entities = map_load_entities("assets/map/light_test.map");
    entities = map_load_entities("assets/map/main.map");

    // Init light
    Model sphere_model = model_load("assets/model/base_model/sphere.obj");
    light = light_init(0.0f, 2.0f, -10.0f);
    light_set_ambient(&light, 0.1f, 0.1f, 0.1f);
    light_set_diffuse(&light, 0.5f, 0.5f, 0.5f);
    light_set_specular(&light, 0.5f, 0.5f, 0.5f);
    
    light_entity = entity_init(&sphere_model);
    light_entity.position[0] = light.position[0];
    light_entity.position[1] = light.position[1];
    light_entity.position[2] = light.position[2];
    
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
    for (int i = 0; i < arrlen(entities); i++) {
        renderer_render(&entity_shader, &light, &entities[i], &camera);
    }

    // Render light
    renderer_render(&light_shader, &light, &light_entity, &camera);
}
