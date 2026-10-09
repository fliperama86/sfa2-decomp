/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b436c_slot04_04(Object *obj);
void func_801b43f4_slot04_04(Object *obj);
u8 func_8013f8c4(Object *obj, int a, int b);
void func_801b4bc4_slot04_04(Object *obj);

void func_801b432c_slot04_04(Object *obj) {
    if (obj->field_07 == 0) {
        func_801b436c_slot04_04(obj);
    } else {
        func_801b43f4_slot04_04(obj);
    }
}

void func_801b436c_slot04_04(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0 && func_8013f8c4(obj, -0x14, 0x14) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        func_801b4bc4_slot04_04(obj);
    }
}
