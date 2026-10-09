/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80149b80(Object *object);

void func_801b4aa8_slot04_0a(Object *obj) {
    obj->field_159 = 1;
    obj->field_67 = 0;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    func_80120554(obj, obj->side, 0x324);
    func_801307e0(obj, 0xb);
}

void func_801b4b04_slot04_0a(Object *obj) {
    if (((Slot04aObj *)obj)->field_3a != 0) {
        obj->field_07++;
        if (obj->field_0b != 0) {
            obj->field_4c = 0xc0000;
            obj->field_54 = -0x8000;
        } else {
            obj->field_4c = 0xfff40000;
            obj->field_54 = 0x8000;
        }
    }
    func_80130efc(obj);
}

void func_801b4b68_slot04_0a(Object *obj) {
    SequenceStep *s;
    FrameRecord *f;
    if (obj->field_67 != 0) {
        if (func_80149b80(obj)) {
            obj->field_07 = 0;
        }
    }
    *(s32 *)&obj->field_10 += obj->field_4c;
    if ((obj->field_4c += obj->field_54) == 0) {
        obj->field_10 = 0;
        obj->field_07++;
        s = obj->sequence;
        s++;
        obj->sequence = s;
        obj->field_38 = s->duration;
        f = obj->frames + obj->sequence->frame_index;
        obj->frame = f;
        obj->field_4a = f->field_0d;
        obj->field_80 = 1;
    }
}
