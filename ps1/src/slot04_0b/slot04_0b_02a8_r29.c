/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2f44_slot04_0b[];
extern void (*data_801c2f4c_slot04_0b[])(Object *);

void update_sequence_or_finish_on_flag_slot04_0b(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b42ac_slot04_0b(Object *obj) {
    data_801c2f44_slot04_0b[obj->state_3](obj);
}

void func_801b42ec_slot04_0b(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_or_finish_on_flag_2_slot04_0b(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void call_handler_by_state_3_set_flag_2_slot04_0b(Object *obj) {
    obj->field_157 = 1;
    data_801c2f4c_slot04_0b[obj->state_3](obj);
}
