/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_slide_decelerate_then_next_step_slot04_07(Object *obj) {
    s16 t = obj->step_flags;
    SequenceStep *s;

    if (t & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else if ((t & 0xff) == 0) {
        update_sequence(obj);
    } else {
        if (obj->facing_side != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
        if (obj->vel_x < 0) {
            s = obj->sequence;
            s = s + 1;
            obj->sequence = s;
            obj->step_timer = s->duration;
            obj->step_flags = s->flags;
            obj->frame = obj->frames + obj->sequence->frame_index;
            obj->frame_sound_index = obj->frame->sequence_param;
        }
    }
}

void start_jump_on_step_flag_by_facing_slot04_07(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->facing_side == 0) {
            obj->vel_x = 0x28000;
        } else {
            obj->vel_x = -0x28000;
        }
        obj->vel_y = 0x58000;
        obj->accel_y = -0x5000;
        obj->accel_x = 0;
        obj->active_state = 1;
    }
    update_sequence(obj);
}
