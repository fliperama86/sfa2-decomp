/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b231c_slot04_09(Object *obj) {
    if (!((s16)obj->field_3a & 0x8000)) {
        func_80130efc(obj);
    } else {
        obj->field_45 = 1;
        obj->field_4c = 0x80000;
        obj->field_50 = 0x80000;
        obj->field_54 = -0x8000;
        obj->field_58 = -0x6000;
        obj->field_07++;
        if (obj->field_0b == 0) {
            obj->field_4c = 0xfff80000;
            obj->field_54 = 0x8000;
        }
        func_801307e0(obj, 0x47);
    }
}
