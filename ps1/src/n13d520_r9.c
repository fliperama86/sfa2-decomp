/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void move_left_set_state_1_when_off_screen(Object *object) {
    object->pos_x = object->pos_x - 9;
    if (object->pos_x < -0x42) object->state_1 = 6;
}

void wait_countdown_then_next_state_1(Object *object) {
    int t = object->countdown - 1;
    object->countdown = t;
    if ((s16)t < 0) object->state_1 = object->state_1 + 1;
}

void move_left_set_state_1_when_off_screen_2(Object *object) {
    object->pos_x = object->pos_x - 9;
    if (object->pos_x < -0x42) object->state_1 = 6;
}
