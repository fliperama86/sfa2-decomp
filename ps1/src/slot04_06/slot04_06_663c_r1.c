/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int move_by_velocity_return_y_minus_ground_slot04_06(Object *obj);

void land_on_ground_and_advance_state_1_slot04_06(Object *obj) {
    if ((move_by_velocity_return_y_minus_ground_slot04_06(obj) << 16) > 0) {
        ((Slot04aObj *)obj)->timer = 8;
        obj->step_timer = 1;
        obj->state_1++;
        obj->pos_y = obj->ground_y;
        advance_sequence_checked(obj);
    }
}

void count_down_timer_advance_state_0_slot04_06(Object *obj) {
    ((Slot04aObj *)obj)->timer--;
    if (((Slot04aObj *)obj)->timer & 0x80) {
        obj->state_0++;
    }
}
