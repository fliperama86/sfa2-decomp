/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e00_slot04_0a(Object *obj);

void func_801b2b60_slot04_0a(Object *obj) {
    s16 y;
    int a;
    func_801b3e00_slot04_0a(obj);
    y = obj->field_70;
    if (obj->pos_y < y) {
        func_80130efc(obj);
    } else {
        obj->pos_y = y;
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->field_07 = 5;
        if (obj->field_12a == 0) {
            a = 0x36;
        } else {
            a = 0x37;
            if (obj->field_12a == 4) {
                obj->field_07 = 3;
                a = 0x38;
            }
        }
        func_801307e0(obj, a);
    }
}

void func_801b2bf4_slot04_0a(Object *obj) {
    int a;
    int b;
    if (((Slot04aObj *)obj)->field_3a != 0) {
        a = 0xe0000;
        b = -0x8000;
        obj->field_07++;
        if (obj->field_0b == 0) {
            a = 0xfff20000;
            b = 0x8000;
        }
        obj->field_4c = a;
        obj->field_54 = b;
    }
    func_80130efc(obj);
}

void func_801b2c4c_slot04_0a(Object *obj) {
    SequenceStep *p;
    FrameRecord *fr;
    *(s32 *)&obj->field_10 += obj->field_4c;
    obj->field_4c += obj->field_54;
    if (obj->field_4c == 0) {
        obj->field_10 = 0;
        obj->field_07++;
        p = obj->sequence;
        p++;
        obj->sequence = p;
        obj->field_38 = p->duration;
        fr = obj->frames + obj->sequence->frame_index;
        obj->frame = fr;
        obj->field_4a = fr->field_0d;
        obj->field_80 = 1;
    }
    func_80130efc(obj);
}
