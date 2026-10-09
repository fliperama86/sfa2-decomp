/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2f98_slot04_05(Object *obj) {
    if (((Slot04aObj *)obj)->field_1a6 != 2) {
        func_801307e0(obj, 0x25);
        obj->field_04 = 1;
        obj->field_06 = 8;
        obj->field_05 = 0;
        obj->field_07 = 0;
        obj->field_15a = 3;
    } else {
        func_801307e0(obj, 0x27);
        obj->field_04 = 1;
        obj->field_06 = 8;
        obj->field_4c = 0xc0000;
        obj->field_54 = -0x8000;
        obj->field_50 = 0x80000;
        obj->field_05 = 0;
        obj->field_07 = 1;
        obj->field_15a = 1;
        obj->field_58 = -0x6000;
        obj->field_45 = 1;
    }
    ((Slot04aObj *)obj)->field_1a6 = 0;
}

int func_801b3048_slot04_05(Object *obj) {
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    return obj->field_4c += obj->field_54;
}
