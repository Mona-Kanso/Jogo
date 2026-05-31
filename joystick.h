#ifndef JOYSTICK_H
#define JOYSTICK_H

typedef struct {
    int left;
    int right;
    int up;
    int down;
} joystick;

joystick *joystick_create();
void      joystick_destroy(joystick *element);

#endif