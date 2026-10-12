/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void shift_pos_y_by_countdown_phase_and_step_state_slot28(Object *obj) {
    int d = -0x8000;
    int t;
    if (obj->variant_index != 0) {
        obj->state_1++;
    }
    t = obj->countdown + 1;
    obj->countdown = t & 7;
    if (t & 4) {
        d = 0x10000;
    }
    ((Slot28Obj *)obj)->pos_y = ((Slot28Obj *)obj)->pos_y - d;
}

void move_down_until_y_c0_then_next_state_1_slot28(Object *obj) {
    ((Slot28Obj *)obj)->pos_y += 0x4000;
    if (obj->pos_y >= 0xc0) {
        obj->pos_y = 0xc0;
        obj->state_1++;
    }
}
