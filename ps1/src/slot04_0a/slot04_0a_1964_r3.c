/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1c48_slot04_0a(Object *obj) {
    int a;
    int b;

    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        a = 0x60000;
        b = -0x4000;
        obj->field_58 = -0x5000;
        obj->field_07++;
        obj->field_50 = a;
        if (obj->field_130 & 0x8000) {
            a = 0x80000;
            obj->field_0b = obj->field_0b ^ 1;
            b = -0x5000;
        }
        if (obj->field_0b != 0) {
            a = -a;
            b = -b;
        }
        obj->field_4c = a;
        obj->field_54 = b;
        func_801307e0(obj, 0x24);
    }
}
