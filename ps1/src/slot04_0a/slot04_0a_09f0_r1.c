/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_state_3_and_set_flags_slot04_0a(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    set_sequence_and_tables_from_direction(obj);
    obj->field_278 = 1;
}

void start_sound_and_horizontal_slide_on_mode_slot04_0a(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    if (obj->mode != 0) {
        o->state_3++;
        play_sound_at(o, o->side, 0x324);
        if (o->facing_side != 0) {
            o->vel_x = 0xc0000;
            o->accel_x = -0x8000;
        } else {
            o->vel_x = 0xfff40000;
            o->accel_x = 0x8000;
        }
    }
    update_sequence(o);
}

void slide_until_vel_x_zero_then_next_step_slot04_0a(Object *obj) {
    SequenceStep *s;
    FrameRecord *f;
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
    if (obj->vel_x == 0) {
        obj->state_3++;
        obj->pos_x_frac = 0;
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
