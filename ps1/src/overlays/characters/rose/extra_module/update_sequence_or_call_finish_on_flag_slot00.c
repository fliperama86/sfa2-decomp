/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_800797b0_slot00[];

void update_sequence_or_call_finish_on_flag_slot00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        update_sequence(obj);
    }
}

void set_state_3_and_sequence_by_variant_slot00(Object *obj) {
    int t = 0xc;
    int s = 1;
    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    obj->field_225 = 1;
    if (obj->counter == 0) {
        obj->field_225 = 0;
        s = obj->direction_index >> 1;
        add_to_meter_clamped(obj, s);
    }
    if (obj->variant_index != 0) {
        t = 0x12;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    }
    set_sequence_by_side_and_mode(obj, (s16)(s + t));
}

void call_handler_by_state_0_slot00(Object *obj) {
    data_800797b0_slot00[obj->state_0](obj);
}
