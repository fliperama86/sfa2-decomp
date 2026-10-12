/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_decelerating_slide_sequence_slot04_07(Object *obj) {
    s16 t = obj->step_flags;
    u16 *q;
    FrameRecord *fr;

    if (t & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if ((t & 0xff) != 0) {
            if (obj->facing_side == 0) {
                *(s32 *)&obj->pos_x_frac -= obj->vel_x;
            } else {
                *(s32 *)&obj->pos_x_frac += obj->vel_x;
            }
            obj->vel_x += obj->accel_x;
            if (obj->vel_x < 0) {
                q = (u16 *)obj->sequence;
                q += 6;
                obj->sequence = (SequenceStep *)q;
                obj->step_timer = q[0];
                obj->step_flags = q[1];
                fr = obj->frames + obj->sequence->frame_index;
                obj->frame = fr;
                obj->frame_sound_index = fr->sequence_param;
                obj->sequence_changed_flag = 1;
            }
        }
        update_sequence(obj);
    }
}

void start_jump_on_step_flag_slot04_07(Object *obj) {
    if ((obj->step_flags & 0xff) != 0) {
        obj->vel_x = 0xfffd8000;
        obj->accel_y = -0x5000;
        obj->vel_y = 0x58000;
        obj->accel_x = 0;
        obj->state_3++;
        if (obj->facing_side == 0) {
            obj->vel_x = 0x28000;
        }
        obj->active_state = 1;
    }
    update_sequence(obj);
}
