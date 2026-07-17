#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

void input_set_key(char key, bool value);
bool input_key_down(char key);
void input_set_mouse_rel_pos(int x, int y);
void input_mouse_rel_pos(int *x, int *y);
void input_reset_mouse_rel_pos();

#endif//INPUT_H
