/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

void apply_velocity_and_acceleration_2_slot2b(Object *obj);

void move_and_animate_until_timer_expires_slot2b(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;
    obj->countdown_timer--;
    if ((obj->countdown_timer & 0x80) != 0) {
        o->state_0++;
    } else {
        apply_velocity_and_acceleration_2_slot2b(o);
        advance_sequence_checked(o);
    }
}
