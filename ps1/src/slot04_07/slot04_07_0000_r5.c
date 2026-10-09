/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0620_slot04_07(Object *obj) {
    int one = 1;

    obj->field_07 = obj->field_07 + 1;
    obj->field_159 = one;
    if (obj->field_12a == 0) {
        ((Slot04aObj *)obj)->field_1ca = one;
    } else if (obj->field_25f == 0 && (obj->field_130 & 0xa000) != 0) {
        if ((u8)func_8013f8c4(obj, -0x12, 0x10) != 0) {
            obj->field_04 = one;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
            return;
        }
        if (obj->field_12a == 2 && (obj->field_130 & 0x8000) != 0 && obj->field_262 != 0) {
            obj->field_07 = 2;
            func_80141f28(obj, 1);
            func_80130ec0(obj);
            obj->field_278 = one;
            obj->field_29a = one;
            func_801307e0(obj, 0x1e);
            return;
        }
    }
    if (obj->field_12a != 4 || ((Slot04aObj *)obj)->field_1ca != 7) {
        func_80130dc0(obj);
    } else {
        func_80130ec0(obj);
        func_801307e0(obj, 0x3c);
    }
}
