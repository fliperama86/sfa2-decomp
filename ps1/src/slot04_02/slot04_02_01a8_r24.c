/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2ab4_slot04_02(Object *obj) {
    int a;

    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((s16)obj->field_46 < 0) {
        a = 0;
        obj->field_165 = 0;
        obj->field_07++;
        if (obj->field_4b == 0) {
            a = 6;
            obj->other->field_6b = 0xa;
        }
        obj->field_27b = a;
        func_801307e0(obj, 0x46);
    } else {
        func_80130efc(obj);
    }
}
