/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s16 *data_801c2060_slot04_07[];

void set_state_3_seven_stop_with_gravity_sequence_slot04_07(Object *obj);

void update_sequence_or_end_set_other_code_2_slot04_07(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    }
}

void start_sequence_from_table_or_fall_slot04_07(Object *obj) {
    int a;

    obj->state_3++;
    obj->active_flag = 0;
    ((Slot04aObj *)obj)->seen_flag = 0;
    a = obj->direction_index >> 1;
    a = data_801c2060_slot04_07[a][(s16)obj->countdown >> 1];
    if (a >= 0) {
        set_sequence_by_side_and_mode(obj, a);
    } else {
        set_state_3_seven_stop_with_gravity_sequence_slot04_07(obj);
    }
}

void set_state_3_seven_stop_with_gravity_sequence_slot04_07(Object *obj) {
    obj->state_3 = 7;
    obj->vel_y = 0;
    obj->accel_y = -0x5600;
    obj->vel_x = 0;
    obj->accel_x = 0;
    set_sequence_by_side(obj, 0x21);
}
