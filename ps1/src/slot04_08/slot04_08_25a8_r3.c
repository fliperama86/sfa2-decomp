/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b286c_slot04_08(Object *obj) {
    Object *other;

    obj->field_46 = (s16)obj->field_46 - 1;
    if (obj->field_46 & 0x8000) {
        other = obj->other;
        obj->field_07++;
        obj->field_0b ^= 1;
        other->field_15b = 1;
        other->field_260 = 1;
        func_80140770(obj, 2, 0xf, -0x200, 0, 0, 0);
        if (((Slot04aObj *)other)->field_5c < 0) {
            obj->field_167 = (obj->field_12a >> 1) + 0xc;
        }
        if (obj->field_0b == 0) {
            obj->field_4c = 0x18000;
        } else {
            obj->field_4c = -0x18000;
        }
        obj->field_50 = -0x50000;
        obj->field_58 = 0x5000;
        obj->field_45 = 1;
        func_80130678(obj, 0x30);
    }
}
