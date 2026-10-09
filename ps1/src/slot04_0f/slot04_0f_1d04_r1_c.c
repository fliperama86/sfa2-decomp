/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5af8_slot04_0f[];

void func_80142adc(Object *object);
void func_801b470c_slot04_0f(Object *obj);

void func_801b2504_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *p;
    s32 y;

    func_80130efc(o);
    if (*(u8 *)&o->field_3a != 0) {
        o->field_07 = o->field_07 + 1;
        p = func_8011f0e8(o);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0xf;
            p->field_03 = 0;
            p->field_66 = o->field_66;
            p->field_65 = o->field_65;
            p->field_ac = o->field_12a;
            p->field_ad = 0;
            p->field_0e = o->field_0e;
            p->field_0b = o->field_0b;
            p->field_0c = o->field_0c;
            p->field_26 = o->field_26;
            *(s32 *)&p->field_10 = *(s32 *)&o->field_10;
            y = *(s32 *)&o->field_14;
            p->field_7a = 0x60;
            p->field_5c = 0;
            p->field_3c = o;
            p->field_7c = 0x1e0;
            *(s32 *)&p->field_14 = y;
            p->field_0d = o->field_0d;
            p->field_90 = o->field_90;
            p->field_98 = o->field_98;
            p->field_9c = o->field_9c;
            o->field_14c = (s32)p;
            obj->field_47 = 5;
            o->field_240++;
        }
        func_801204f4(o, o->side, 0x10);
    }
}

void func_801b2644_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if ((s16)o->field_3a < 0) {
        func_801b470c_slot04_0f(o);
    } else {
        if (obj->field_47 != 0) {
            obj->field_47--;
            if (obj->field_47 != 0) {
                goto tail;
            }
            o->field_17b = 0;
        }
        func_80142adc(o);
    tail:
        func_80130efc(o);
    }
}

void func_801b26cc_slot04_0f(Object *obj) {
    data_801c5af8_slot04_0f[obj->field_07](obj);
}
