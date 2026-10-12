/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_8007a11c_slot00[];
extern ObjectFn data_8007a124_slot00[];
void apply_velocity_and_acceleration_slot00(Object *o);

void call_handler_by_state_2_slot00(Object *obj) {
    data_8007a11c_slot00[obj->state_2](obj);
}

void count_down_to_state_2_move_and_advance_slot00(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->countdown = 8;
        obj->state_2++;
    }
    apply_velocity_and_acceleration_slot00(obj);
    advance_sequence_checked(obj);
}

void blink_active_flag_until_countdown_ends_slot00(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->state_0++;
    }
    if (obj->countdown & 1) {
        obj->active_flag ^= 1;
    }
    advance_sequence_checked(obj);
}

void call_handler_by_state_2_2_slot00(Object *obj) {
    data_8007a124_slot00[obj->state_2](obj);
}
