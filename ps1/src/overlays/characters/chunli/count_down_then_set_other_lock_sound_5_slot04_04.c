/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c4374_slot04_04[];

void count_down_then_set_other_lock_sound_5_slot04_04(Object *obj) {
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->other_lock = data_801c4374_slot04_04[3];
        } else {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 10;
            obj->other_lock = data_801c4374_slot04_04[obj->direction_index >> 1];
        }
        obj->field_165 = 0;
        play_side_sound_at(obj, obj->side, 5);
    }
}

void slide_x_then_end_sequence_reset_slot04_04(Object *obj) {
    u16 t;

    if (obj->vel_x != 0) {
        if (obj->facing_side != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        }
        obj->vel_x = obj->vel_x - obj->accel_x;
        if (obj->vel_x < 0) {
            obj->vel_x = 0;
        }
    }
    t = obj->step_flags;
    if (t & 0xff) {
        obj->step_flags = t & 0xff00;
        obj->vel_x = obj->vel_y;
        obj->accel_x = obj->accel_y;
    }
    if ((s16)obj->step_flags & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
