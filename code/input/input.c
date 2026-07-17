#include "input.h"

static bool keys[512];
static int mouse_rel_x;
static int mouse_rel_y;

void input_set_key(char key, bool value)
{
    keys[(int)key] = value;
}

bool input_key_down(char key)
{
    return keys[(int)key];
}

void input_set_mouse_rel_pos(int x, int y)
{
    mouse_rel_x = x;
    mouse_rel_y = y;
}

void input_mouse_rel_pos(int *x, int *y)
{
    *x = mouse_rel_x;
    *y = mouse_rel_y;
}

void input_reset_mouse_rel_pos()
{
    mouse_rel_x = 0;
    mouse_rel_y = 0;
}
