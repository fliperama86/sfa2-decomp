/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void move_pos_x_left_half_unit_slot28(Object *obj) {
    *(s32 *)&obj->pos_x_frac += 0xffff8000;
}
