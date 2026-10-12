/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void move_down_to_y_limit_then_advance_state_1_slot28(Object *obj) {
    *(u16 *)&obj->pos_y += 0x10;
    if (obj->pos_y >= 0x61) {
        obj->countdown = 4;
        obj->pos_y = 0x60;
        obj->state_1++;
    }
}

void count_down_then_set_spawn_y_advance_state_1_slot28(Object *obj) {
    int t = obj->countdown - 1;
    obj->countdown = t;
    if ((s16)t == 0) {
        obj->countdown = 4;
        obj->spawn_y = 0x1e2;
        obj->state_1++;
    }
}

void count_down_then_set_byte_advance_state_1_slot28(Object *obj) {
    int t = obj->countdown - 1;
    obj->countdown = t;
    if ((s16)t == 0) {
        obj->field_6b = 0xff;
        obj->state_1++;
    }
}

void advance_state_1_while_countdown_nonzero_slot28(Object *obj) {
    int t = obj->countdown - 1;
    obj->countdown = t;
    if ((s16)t != 0) {
        obj->state_1++;
    }
}
