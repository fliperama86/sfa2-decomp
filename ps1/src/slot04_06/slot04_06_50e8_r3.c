/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013f8c4(Object *obj, int a, int b);

void func_801b55cc_slot04_06(Object *obj) {
    obj->field_07++;
    if (obj->field_218 != 0) {
        if (func_8013f8c4(obj, -0x18, 0x14) != 0) {
            obj->field_04 = 1;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
        }
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}
