#ifndef ENTITY_H
#define ENTITY_H

#include "model.h"

typedef struct {
    Model model;
    float position[3];
    float rotation[3];
    float scale;
} Entity;

void entity_set_position(Entity *entity, float x, float y, float z);
void entity_set_rotation(Entity *entity, float x, float y, float z);
void entity_increase_position(Entity *entity, float dx, float dy, float dz);
void entity_increase_rotation(Entity *entity, float dx, float dy, float dz);

#endif//ENTITY_H
