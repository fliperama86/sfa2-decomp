/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2978_slot04_0d[];

void update_sequence_with_input_check(Object *object);
void set_sequence_and_tables_from_direction(Object *object);

/* functions of other units of this module */

void load_velocities_from_table_2_slot04_0d(Object *obj) {
    update_sequence_with_input_check(obj);
}

void update_sequence_with_flagged_pos_x_nudge_slot04_0d(Object *obj) {
    s16 t = obj->step_flags;
    int d;

    if (t >= 0) {
        if ((t & 0xff) == 0) {
            update_sequence(obj);
        } else {
            if (obj->facing_side == 0) {
                d = -2;
            } else {
                d = 2;
            }
            obj->pos_x = d + obj->pos_x;
            update_sequence(obj);
        }
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_handler_by_state_3_set_flag_slot04_0d(Object *obj) {
    obj->field_157 = 1;
    data_801c2978_slot04_0d[obj->state_3](obj);
}

void advance_state_3_and_start_sequence_slot04_0d(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void load_velocities_from_table_3_slot04_0d(Object *obj) {
    update_sequence_with_input_check(obj);
}
