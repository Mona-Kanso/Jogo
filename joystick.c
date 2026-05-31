#include <stdlib.h>
#include "joystick.h"

joystick *joystick_create() {
    joystick *j = malloc(sizeof(joystick));
    j->left = j->right = j->up = j->down = 0;
    return j;
}

void joystick_destroy(joystick *element) {
    free(element);
}