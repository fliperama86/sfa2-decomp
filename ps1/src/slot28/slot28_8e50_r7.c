/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_up_to_y_limit_then_advance_state_1_slot28(Object *obj) {
    int t = *(u16 *)&obj->pos_y - 0x10;
    obj->pos_y = t;
    if ((s16)t < 0xa0) {
        obj->countdown = 4;
        obj->pos_y = 0xa0;
        obj->state_1++;
    }
}

void count_down_then_reload_and_set_spawn_y_slot28(Object *obj) {
    int t = obj->countdown - 1;
    obj->countdown = t;
    if ((s16)t == 0) {
        obj->countdown = 0x20;
        obj->spawn_y = 0x1e2;
        obj->state_1++;
    }
}

void count_down_then_advance_state_1_slot28(Object *obj) {
    int t = obj->countdown - 1;
    obj->countdown = t;
    if ((s16)t == 0) {
        obj->state_1++;
    }
}
