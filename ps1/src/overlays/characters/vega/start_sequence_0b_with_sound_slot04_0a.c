/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_sequence_0b_with_sound_slot04_0a(Object *obj) {
    obj->active_flag_3 = 1;
    obj->active_flag = 0;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    play_sound_at(obj, obj->side, 0x324);
    set_sequence_by_side_and_mode(obj, 0xb);
}

void start_slide_on_mode_no_sound_slot04_0a(Object *obj) {
    if (((Slot04aObj *)obj)->mode != 0) {
        obj->state_3++;
        if (obj->facing_side != 0) {
            obj->vel_x = 0xc0000;
            obj->accel_x = -0x8000;
        } else {
            obj->vel_x = 0xfff40000;
            obj->accel_x = 0x8000;
        }
    }
    update_sequence(obj);
}

void slide_until_stopped_then_advance_step_slot04_0a(Object *obj) {
    SequenceStep *s;
    FrameRecord *f;
    if (obj->active_flag != 0) {
        if ((u8)consume_active_frame_flag(obj)) {
            obj->state_3 = 0;
        }
    }
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    if ((obj->vel_x += obj->accel_x) == 0) {
        obj->pos_x_frac = 0;
        obj->state_3++;
        s = obj->sequence;
        s++;
        obj->sequence = s;
        obj->step_timer = s->duration;
        f = obj->frames + obj->sequence->frame_index;
        obj->frame = f;
        obj->frame_sound_index = f->sequence_param;
        obj->sequence_changed_flag = 1;
    }
}
