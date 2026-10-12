/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_down_until_pos_y_limit_advance_state_1_slot28(Object *obj) {
    *(int *)&obj->pos_y_frac += 0x8000;
    if (obj->pos_y >= 0xc0) {
        obj->pos_y = 0xc0;
        obj->state_1++;
    }
}
