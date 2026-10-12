/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c5478_slot04_06[];

void set_sequence_45_or_reset_state_3_slot04_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x45);
    } else {
        obj->state_3 = 0;
    }
}

void start_grab_setting_other_states_slot04_06(Object *obj) {
    Object *other;
    obj->state_3++;
    other = obj->other;
    obj->substate_index = 0;
    obj->field_12d = 0;
    obj->step_count_2 = 0;
    obj->step_limit = 0;
    other->state_0 = 1;
    other->state_1 = 3;
    other->state_2 = 0;
    other->state_3 = 0;
    other->status_flag = 0xff;
    other->field_15b = 1;
    other->gauge = 0x64;
    obj->vel_x = 0xa0000;
    obj->accel_x = -0xe000;
    obj->status_flag = 1;
    obj->gauge = 0x80;
    ((Slot04aObj *)obj)->table_value = data_801c5478_slot04_06[obj->direction_index >> 1];
    set_sequence_by_side_and_mode(obj, 0x2c);
}
