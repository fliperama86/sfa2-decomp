/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c3dec_slot04_08[];

void update_sequence_or_finish_on_flag_slot04_08(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_table_function_by_state_3_17_slot04_08(Object *obj) {
    data_801c3dec_slot04_08[obj->state_3](obj);
}
