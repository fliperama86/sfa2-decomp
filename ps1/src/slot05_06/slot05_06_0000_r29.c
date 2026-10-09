/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801dd404_slot05_06[];
int func_80140cd8(Object *object, int a, int b);

void func_801ca568_slot05_06(Object *obj) {
    Slot04aObj *s = (Slot04aObj *)obj;

    s->field_1c2 = s->field_1c2 - 1;
    if (s->field_1c2 & 0x80) {
        obj->field_4c = 0x18000;
        obj->field_50 = 0x8a000;
        obj->field_54 = 0;
        obj->field_58 = -0x6000;
        obj->field_07 = obj->field_07 + 1;
        func_80140cd8(obj, *(s16 *)(data_801dd404_slot05_06 + (obj->field_12a & 0xfe)), 0);
        func_801307e0(obj, 0x23);
    } else {
        func_80130efc(obj);
    }
}

void func_801ca610_slot05_06(Object *o) {
    if (*(u8 *)&o->field_3a != 0) {
        o->field_45 = 1;
        ((Slot04aObj *)o)->field_1c8 = 0;
        o->field_07++;
    }
    func_80130efc(o);
}
