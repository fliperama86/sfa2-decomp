/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4b8c_slot04_07(Object *obj) {
    s16 t = obj->field_3a;
    SequenceStep *s;

    if (t & 0x8000) {
        func_80131468(obj);
    } else if ((t & 0xff) == 0) {
        func_80130efc(obj);
    } else {
        if (obj->field_0b != 0) {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        } else {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
        }
        obj->field_4c = obj->field_4c + obj->field_54;
        if (obj->field_4c < 0) {
            s = obj->sequence;
            s = s + 1;
            obj->sequence = s;
            obj->field_38 = s->duration;
            obj->field_3a = s->flags;
            obj->frame = obj->frames + obj->sequence->frame_index;
            obj->field_4a = obj->frame->field_0d;
        }
    }
}

void func_801b4c78_slot04_07(Object *obj) {
    if ((u8)obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_0b == 0) {
            obj->field_4c = 0x28000;
        } else {
            obj->field_4c = -0x28000;
        }
        obj->field_50 = 0x58000;
        obj->field_58 = -0x5000;
        obj->field_54 = 0;
        obj->field_45 = 1;
    }
    func_80130efc(obj);
}
