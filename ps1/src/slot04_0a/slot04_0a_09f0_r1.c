/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b09f0_slot04_0a(Object *obj) {
    obj->field_159 = 1;
    obj->field_07++;
    func_80130dc0(obj);
    obj->field_278 = 1;
}

void func_801b0a38_slot04_0a(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    if (obj->field_3a != 0) {
        o->field_07++;
        func_80120554(o, o->side, 0x324);
        if (o->field_0b != 0) {
            o->field_4c = 0xc0000;
            o->field_54 = -0x8000;
        } else {
            o->field_4c = 0xfff40000;
            o->field_54 = 0x8000;
        }
    }
    func_80130efc(o);
}

void func_801b0ab4_slot04_0a(Object *obj) {
    SequenceStep *s;
    FrameRecord *f;
    *(s32 *)&obj->field_10 += obj->field_4c;
    obj->field_4c += obj->field_54;
    if (obj->field_4c == 0) {
        obj->field_07++;
        obj->field_10 = 0;
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
