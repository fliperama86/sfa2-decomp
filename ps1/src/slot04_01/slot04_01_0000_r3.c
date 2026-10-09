/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"


u8 func_8013f8c4(Object *object, int a, int b);

void func_801b03a0_slot04_01(Object *obj) {
    if (obj->field_07 == 0) {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_12a != 0 && obj->field_25f == 0 && (obj->field_130 & 0xa000) != 0) {
            if (func_8013f8c4(obj, -0x14, 0x14) != 0) {
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
    } else {
        func_80142a14(obj);
    }
}
