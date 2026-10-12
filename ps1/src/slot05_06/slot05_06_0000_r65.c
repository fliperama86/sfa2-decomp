/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cd998_slot05_06(Object *obj) {
    obj->vel_x = 0xa0000;
    obj->accel_x = -0xc000;
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}

void update_slide_with_sound_by_step_flags_slot05_06(Object *obj) {
    int d;

    if ((s16)obj->step_flags >= 0) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if ((s16)obj->step_flags == 3) {
            play_sound_at(obj, ((Slot04aObj *)obj)->side, 0x324);
        }
        if ((s16)obj->step_flags == 2 && (s32)obj->vel_x >= 0) {
            d = obj->vel_x;
            if (obj->facing_side == 0) {
                d = -d;
            }
            *(s32 *)&obj->pos_x_frac += d;
            obj->vel_x += obj->accel_x;
            update_sequence(obj);
        } else {
            if (((Slot04aObj *)obj)->mode != 0 && (consume_active_frame_flag(obj) & 0xff) != 0) {
                obj->state_3 = 0;
            }
            update_sequence(obj);
        }
    }
}
