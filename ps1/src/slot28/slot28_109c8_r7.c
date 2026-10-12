/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_up_until_y_a0_then_next_state_1_slot28(Object *obj) {
    ((Slot28Obj *)obj)->pos_y -= 0x2000;
    if (obj->pos_y < 0xa0) {
        obj->pos_y = 0xa0;
        obj->state_1++;
    }
}
