/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *obj);

void func_801b51e0_slot04_09(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *p = o->other;
    u8 *c;

    o->field_07++;
    c = (u8 *)obj->field_330;
    c[4] = 2;
    c[5] = 0;
    c[6] = 0;
    c[7] = 0;
    p->pos_y = p->field_70;
    p->field_0c = obj->field_334;
    p->field_0d = obj->field_335;
    o->field_4c = 0x40000;
    o->field_50 = 0;
    o->field_54 = 0;
    o->field_58 = -0x6000;
    if (o->field_0b != 0) {
        o->field_4c = 0x40000;
    } else {
        o->field_4c = 0xfffc0000;
    }
    p->field_0b = p->field_0b ^ 1;
    func_801307e0(o, 0x1a);
}

void func_801b5288_slot04_09(Object *o) {
    func_801b2858_slot04_09(o);
    if (o->pos_y < o->field_70) {
        func_80130efc(o);
    } else {
        o->field_07++;
        o->field_14 = 0;
        o->pos_y = o->field_70;
        func_801307e0(o, 0x1b);
    }
}

void func_801b52f8_slot04_09(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_0b = obj->field_0b ^ 1;
        obj->field_73 = 0;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
