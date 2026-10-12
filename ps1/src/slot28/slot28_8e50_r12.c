/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_left_to_x_c8_then_advance_state_1_slot28(Object *obj) {
    *(s32 *)&obj->pos_x_frac -= 0x4000;
    if (obj->pos_x <= 0xc8) {
        obj->pos_x = 0xc8;
        obj->pos_y = 0xa0;
        obj->state_1++;
    }
}
