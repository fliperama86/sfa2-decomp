/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_by_state_3_zero_restore_facing_or_update_slot04_04(Object *obj);
void call_table_function_by_state_3_13_slot04_04(Object *obj);

void update_sequence_or_finish_on_flag_2_slot04_04(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_state_3_dispatch_by_input_dir_second_a_slot04_04(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_13_slot04_04(obj);
    } else {
        call_by_state_3_zero_restore_facing_or_update_slot04_04(obj);
    }
}
