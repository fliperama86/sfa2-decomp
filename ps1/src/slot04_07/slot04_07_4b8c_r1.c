/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4b8c_slot04_07(Object *obj) {
    s16 t = obj->step_flags;
    SequenceStep *s;

    if (t & 0x8000) {
        func_80131468(obj);
    } else if ((t & 0xff) == 0) {
        update_sequence(obj);
    } else {
        if (obj->field_0b != 0) {
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
            obj->field_4a = obj->frame->field_0d;
        }
    }
}

void func_801b4c78_slot04_07(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_0b == 0) {
            obj->vel_x = 0x28000;
        } else {
            obj->vel_x = -0x28000;
        }
        obj->vel_y = 0x58000;
        obj->accel_y = -0x5000;
        obj->accel_x = 0;
        obj->field_45 = 1;
    }
    update_sequence(obj);
}
