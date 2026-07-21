#include "renderer.h"
#include <stdio.h>
#include <glad/glad.h>
#include <stb_ds.h>
#include <cglm/cglm.h>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

void renderer_init()
{
    // Setting
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void renderer_clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void create_model_mat(mat4 model_mat, float *position, float *rotation, float scale)
{
    glm_mat4_identity(model_mat);
    glm_translate(model_mat, (vec3){position[0], position[1], position[2]});
    glm_rotate(model_mat, glm_rad(rotation[0]), (vec3){1, 0, 0});
    glm_rotate(model_mat, glm_rad(rotation[1]), (vec3){0, 1, 0});
    glm_rotate(model_mat, glm_rad(rotation[2]), (vec3){0, 0, 1});
    glm_scale(model_mat, (vec3){scale, scale, scale});
}

void create_view_mat(mat4 lookat_mat, vec3 position, vec3 target, vec3 up)
{
    glm_mat4_identity(lookat_mat);
    glm_lookat(position, target, up, lookat_mat);
}

void create_projection_mat(mat4 projection_mat, int window_width, int window_height)
{   
    glm_mat4_identity(projection_mat);
    glm_perspective(glm_rad(45.0f), (float)window_width / (float)window_height, 0.1f, 100.0f, projection_mat);
}

void renderer_render(Shader *shader, DirectionalLight *dir_light, PointLight *point_lights, SpotLight *spot_lights,
		     Entity *entity, Camera *camera)
{
    Model model = entity->model;
    
    glUseProgram(shader->program_id);

    // Model matrix
    mat4 model_mat;
    create_model_mat(
        model_mat, 
        entity->position,
        entity->rotation, 
        entity->scale
    );
    glUniformMatrix4fv(glGetUniformLocation(shader->program_id, "model_mat"), 1, GL_FALSE, (float*)model_mat);

    // View Matrix
    mat4 view_mat;
    create_view_mat(
        view_mat,
        camera->position,
        camera->target,
        camera->up
    );
    glUniformMatrix4fv(glGetUniformLocation(shader->program_id, "view_mat"), 1, GL_FALSE, (float*)view_mat);

    // Projection Matrix
    mat4 projection_mat;
    create_projection_mat(projection_mat, WINDOW_WIDTH, WINDOW_HEIGHT);
    glUniformMatrix4fv(glGetUniformLocation(shader->program_id, "projection_mat"), 1, GL_FALSE, (float*)projection_mat);

    // Dir light
    glUniform3fv(glGetUniformLocation(shader->program_id, "dir_light.direction"), 1, dir_light->direction);
    glUniform3fv(glGetUniformLocation(shader->program_id, "dir_light.color"), 1, dir_light->color);

    // Point light
    for (int i = 0; i < arrlen(point_lights); i++) {
	char uniform_name[64];
	
	sprintf(uniform_name, "point_lights[%d].position", i);
	glUniform3fv(glGetUniformLocation(shader->program_id, uniform_name), 1, point_lights[i].position);
	sprintf(uniform_name, "point_lights[%d].color", i);
	glUniform3fv(glGetUniformLocation(shader->program_id, uniform_name), 1, point_lights[i].color);
	sprintf(uniform_name, "point_lights[%d].constant", i);
	glUniform1f(glGetUniformLocation(shader->program_id, uniform_name), point_lights[i].constant);
	sprintf(uniform_name, "point_lights[%d].linear", i);
	glUniform1f(glGetUniformLocation(shader->program_id, uniform_name), point_lights[i].linear);
	sprintf(uniform_name, "point_lights[%d].quadratic", i);
	glUniform1f(glGetUniformLocation(shader->program_id, uniform_name), point_lights[i].quadratic);
    }

    // Spot light
    for (int i = 0; i < arrlen(spot_lights); i++) {
	char uniform_name[64];
	
	sprintf(uniform_name, "spot_lights[%d].position", i);
	glUniform3fv(glGetUniformLocation(shader->program_id, uniform_name), 1, spot_lights[i].position);
	sprintf(uniform_name, "spot_lights[%d].direction", i);
	glUniform3fv(glGetUniformLocation(shader->program_id, uniform_name), 1, spot_lights[i].direction);
	sprintf(uniform_name, "spot_lights[%d].color", i);
	glUniform3fv(glGetUniformLocation(shader->program_id, uniform_name), 1, spot_lights[i].color);
	sprintf(uniform_name, "spot_lights[%d].constant", i);
	glUniform1f(glGetUniformLocation(shader->program_id, uniform_name), spot_lights[i].constant);
	sprintf(uniform_name, "spot_lights[%d].linear", i);
	glUniform1f(glGetUniformLocation(shader->program_id, uniform_name), spot_lights[i].linear);
	sprintf(uniform_name, "spot_lights[%d].quadratic", i);
	glUniform1f(glGetUniformLocation(shader->program_id, uniform_name), spot_lights[i].quadratic);
	sprintf(uniform_name, "spot_lights[%d].cut_off", i);
	glUniform1f(glGetUniformLocation(shader->program_id, uniform_name), spot_lights[i].cut_off);
	sprintf(uniform_name, "spot_lights[%d].outer_cut_off", i);
	glUniform1f(glGetUniformLocation(shader->program_id, uniform_name), spot_lights[i].outer_cut_off);
    }
    
    // Material
    glUniform3fv(glGetUniformLocation(shader->program_id, "view_pos"), 1, camera->position);
    
    glUniform1f(glGetUniformLocation(shader->program_id, "material.shininess"), 64.0f);

    Texture diffuse_texture = model.diffuse_texture;
    glUniform1i(glGetUniformLocation(shader->program_id, "material.diffuse"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture.texture_id);

    Texture specular_texture = model.specular_texture;
    glUniform1i(glGetUniformLocation(shader->program_id, "material.specular"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular_texture.texture_id);

    // Mesh
    Mesh mesh = model.mesh;
    glBindVertexArray(mesh.vao_id);
    glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, 0);
}

void renderer_render_sphere(Shader *shader, vec3 position, vec3 color, Camera *camera)
{
    Model model = model_load("assets/model/base_model/sphere.obj");
    
    glUseProgram(shader->program_id);

    // Model matrix
    float light_rotation[3] = {0, 0, 0};
    mat4 model_mat;
    create_model_mat(
        model_mat,
        position,
        light_rotation,
        0.1f
    );
    glUniformMatrix4fv(glGetUniformLocation(shader->program_id, "model_mat"), 1, GL_FALSE, (float*)model_mat);

    // View Matrix
    mat4 view_mat;
    create_view_mat(
        view_mat,
        camera->position,
        camera->target,
        camera->up
    );
    glUniformMatrix4fv(glGetUniformLocation(shader->program_id, "view_mat"), 1, GL_FALSE, (float*)view_mat);

    // Projection Matrix
    mat4 projection_mat;
    create_projection_mat(projection_mat, WINDOW_WIDTH, WINDOW_HEIGHT);
    glUniformMatrix4fv(glGetUniformLocation(shader->program_id, "projection_mat"), 1, GL_FALSE, (float*)projection_mat);

    // Material
    glUniform3fv(glGetUniformLocation(shader->program_id, "light_color"), 1, color);

    // Mesh
    Mesh mesh = model.mesh;
    glBindVertexArray(mesh.vao_id);
    glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, 0);
}
