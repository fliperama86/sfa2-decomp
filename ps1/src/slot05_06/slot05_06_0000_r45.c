/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd498_slot05_06[];
void update_sequence_or_end_set_other_code_slot05_06(Object *object);
extern ObjectFn data_801dd4a0_slot05_06[];

void move_x_until_step_index_expires_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int v;

    obj->step_index -= 1;
    if (obj->step_index & 0x80) {
        o->state_3++;
        set_sequence_by_side_and_mode(o, 0x33);
    } else {
        v = 0x80000;
        if (o->facing_side == 0) {
            v = -0x80000;
        }
        *(s32 *)&o->pos_x_frac = v + *(s32 *)&o->pos_x_frac;
        update_sequence(o);
    }
}

void call_table_function_by_state_3_14_slot05_06(Object *obj) {
    data_801dd498_slot05_06[obj->state_3](obj);
}

void start_sequence_3c_and_decrement_remaining_slot05_06(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    obj->field_157 = 0;
    set_sequence_by_side_and_mode(obj, 0x3c);
}

void update_sequence_or_finish_on_status_bit_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    if (obj->status_flags & 0x80) {
        update_sequence_or_end_set_other_code_slot05_06(o);
    } else {
        update_sequence(o);
    }
}

void call_table_function_by_state_3_15_slot05_06(Object *obj) {
    data_801dd4a0_slot05_06[obj->state_3](obj);
}
