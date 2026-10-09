/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801483a4(Object *object, int a_arg, int b_arg);

void func_801b3d94_slot04_02(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    o->field_46 = (s16)o->field_46 - 0x100;
    if (obj->field_3a != 0) {
        o->field_07++;
        if (o->field_4b != 0) {
            o->field_165 = 1;
        } else {
            o->field_165 = 0xff;
        }
        o->field_46 = (o->field_46 & 0xff00) | 0x30;
        if (o->field_0b != 0) {
            o->field_4c = 0x48000;
        } else {
            o->field_4c = -0x48000;
        }
        func_80120554(o, o->side, 0x31c);
        func_801483a4(o, -0x10, 0x58);
    }
    func_80130efc(o);
}

void func_801b3e40_slot04_02(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    o->field_46 = (s16)o->field_46 - 0x100;
    if (o->field_46 & 0x8000) {
        o->field_165 = 0;
        o->field_07++;
        o->field_46 = obj->field_46 | 0x400;
        func_801204f4(o, o->side, 0xc);
    }
    func_80130efc(o);
}

void func_801b3eac_slot04_02(Object *o) {
    o->field_46 = (s16)o->field_46 - 0x100;
    if ((o->field_46 & 0xff00) != 0) {
        o->field_07++;
    }
    func_80130efc(o);
}
