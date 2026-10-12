/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801c3e98_slot04_08;

void update_motion_repeat_sequence_then_branch_end_slot04_08(Object *obj) {
    int k;

    if (((Slot04aObj *)obj)->sub_index != 2) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        obj->vel_x = obj->vel_x + obj->accel_x;
    }
    if ((s16)obj->step_flags & 0x8000) {
        ((Slot04aObj *)obj)->sub_index++;
        if (((Slot04aObj *)obj)->sub_index == 3) {
            if (obj->direction_index != 2 && obj->other->field_163 == 0) {
                reset_object_to_state_1_0_sequence_0(obj);
            } else {
                obj->state_3 = 4;
                k = 0x4f;
                if (obj->direction_index != 2) {
                    k = 0x50;
                }
                set_sequence_by_side_and_mode(obj, k);
            }
        } else {
            obj->state_3 = 1;
            set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x43);
        }
    } else {
        update_sequence(obj);
    }
}

void start_slide_when_step_flags_negative_slot04_08(Object *obj) {
    int x;

    if ((s16)obj->step_flags & 0x8000) {
        obj->vel_x = 0x80000;
        obj->accel_x = -0xc000;
        obj->state_3++;
        x = 0x160;
        if (obj->facing_side == 0) {
            x = -0x160;
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        }
        data_801c3e98_slot04_08 = x + obj->pos_x;
    }
    update_sequence(obj);
}
