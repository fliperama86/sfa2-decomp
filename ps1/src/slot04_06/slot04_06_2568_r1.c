/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c5408_slot04_06[];

void func_801b2568_slot04_06(Object *obj) {
    Slot04aObj *s = (Slot04aObj *)obj;

    s->field_1c2 = s->field_1c2 - 1;
    if (s->field_1c2 & 0x80) {
        obj->field_4c = 0x18000;
        obj->field_50 = 0x8a000;
        obj->field_54 = 0;
        obj->field_58 = -0x6000;
        obj->field_07 = obj->field_07 + 1;
        func_80140cd8(obj, *(s16 *)(data_801c5408_slot04_06 + (obj->field_12a & 0xfe)), 0);
        func_801307e0(obj, 0x23);
    } else {
        func_80130efc(obj);
    }
}
