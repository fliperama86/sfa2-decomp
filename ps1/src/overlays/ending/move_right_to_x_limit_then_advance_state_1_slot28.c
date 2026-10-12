/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void move_right_to_x_limit_then_advance_state_1_slot28(Object *obj) {
    *(u16 *)&obj->pos_x += 4;
    if (obj->pos_x >= 0x88) {
        obj->pos_x = 0x88;
        obj->state_1++;
    }
}
