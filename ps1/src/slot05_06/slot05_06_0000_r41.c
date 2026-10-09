/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801dd474_slot05_06[];

void func_801cb46c_slot05_06(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        obj->field_07++;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x45);
    } else {
        obj->field_07 = 0;
    }
}

void func_801cb4bc_slot05_06(Object *obj) {
    Object *other;
    obj->field_07++;
    other = obj->other;
    obj->field_12c = 0;
    obj->field_12d = 0;
    obj->field_12e = 0;
    obj->field_12f = 0;
    other->field_04 = 1;
    other->field_05 = 3;
    other->field_06 = 0;
    other->field_07 = 0;
    other->field_73 = 0xff;
    other->field_15b = 1;
    other->field_160 = 0x64;
    obj->field_4c = 0xa0000;
    obj->field_54 = -0xe000;
    obj->field_73 = 1;
    obj->field_160 = 0x80;
    ((Slot04aObj *)obj)->field_1c5 = data_801dd474_slot05_06[obj->field_12a >> 1];
    func_801307e0(obj, 0x2c);
}
