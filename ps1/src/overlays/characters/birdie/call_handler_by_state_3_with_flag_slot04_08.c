/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801c3df4_slot04_08[])(Object *, Object *);

/* The call of data_801c3df4_slot04_08 passes one argument although its entries take two: the original does not set the second argument register before it. Written with a second parameter passed on, this function differs from the original in 5 instruction slots. */
void call_handler_by_state_3_with_flag_slot04_08(Object *obj) {
    obj->field_157 = 1;
    ((void (*)(Object *))data_801c3df4_slot04_08[obj->state_3])(obj);
}

void restore_facing_and_advance_state_3_slot04_08(Object *obj, Object *unused) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_or_reset_on_active_frame_slot04_08(Object *obj, Object *unused) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}
