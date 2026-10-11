/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4aa8_slot04_0a(Object *obj) {
    obj->field_159 = 1;
    obj->field_67 = 0;
    obj->state_3++;
    obj->field_0b = obj->field_158;
    play_sound_at(obj, obj->side, 0x324);
    func_801307e0(obj, 0xb);
}

void func_801b4b04_slot04_0a(Object *obj) {
    if (((Slot04aObj *)obj)->field_3a != 0) {
        obj->state_3++;
        if (obj->field_0b != 0) {
            obj->vel_x = 0xc0000;
            obj->accel_x = -0x8000;
        } else {
            obj->vel_x = 0xfff40000;
            obj->accel_x = 0x8000;
        }
    }
    update_sequence(obj);
}

void func_801b4b68_slot04_0a(Object *obj) {
    SequenceStep *s;
    FrameRecord *f;
    if (obj->field_67 != 0) {
        if ((u8)func_80149b80(obj)) {
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
        obj->field_4a = f->field_0d;
        obj->field_80 = 1;
    }
}
