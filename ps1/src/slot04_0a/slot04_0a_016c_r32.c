/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4868_slot04_0a(Object *obj);
void call_handler_by_direction_index_4_slot04_0a(Object *obj);

void update_sequence_or_finish_on_flag_2_slot04_0a(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_handler_by_input_dir_1_slot04_0a(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second == 0) {
        func_801b4868_slot04_0a(obj);
    } else {
        call_handler_by_direction_index_4_slot04_0a(obj);
    }
}
