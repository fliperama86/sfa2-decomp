/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0498_slot04_08(Object *obj) {
    u8 one;

    obj->field_07++;
    if (obj->field_12a != 0 && (obj->field_130 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x21, 0xd) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        one = 1;
        obj->field_159 = one;
        func_80130dc0(obj);
        if (obj->field_12a == 4) {
            obj->field_29a = one;
        }
    }
}

void func_801b053c_slot04_08(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        if (obj->field_12a != 4 || obj->field_49 == 0) {
            if ((u8)func_801412a4(obj) != 0) {
                obj->field_07 = 0;
            }
        }
        func_80130efc(obj);
    }
}
