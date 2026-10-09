/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b046c_slot04_04(Object *obj) {
    if (obj->field_07 != 0) {
        func_80142a14(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_12a != 0 && obj->field_25f == 0 && (obj->field_130 & 0xa000) != 0) {
            if ((u8)func_8013f8c4(obj, -0x14, 0xe) != 0) {
                obj->field_04 = 1;
                obj->field_05 = 2;
                obj->field_06 = 0;
                obj->field_07 = 0;
            } else {
                obj->field_159 = 1;
                func_80130dc0(obj);
            }
        } else {
            obj->field_159 = 1;
            func_80130dc0(obj);
        }
    }
}

void func_801b0528_slot04_04(Object *obj) {
    if (obj->field_07 != 0) {
        func_80142a14(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_12a == 4) {
            func_801204f4(obj, obj->side, 0xb);
        }
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}
