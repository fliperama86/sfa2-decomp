/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_80142ba0(Object *object);

int func_801b0d18_slot04_08(Object *obj) {
    int r = 0;
    u8 one;

    if (func_801417cc(obj) != 0) {
        r = 1;
        one = 1;
        obj->field_04 = one;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_129 = 2;
        obj->field_159 = one;
        obj->field_15a = one;
        obj->field_12a = ((Slot04aObj *)obj)->field_2e5;
        obj->field_0b = obj->field_158;
    }
    return r;
}

int func_801b0d94_slot04_08(Object *obj) {
    int r = 0;
    u8 one;

    if (func_801417cc(obj) != 0) {
        r = 1;
        one = 1;
        obj->field_04 = one;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_129 = 4;
        obj->field_159 = one;
        obj->field_15a = one;
        obj->field_12a = ((Slot04aObj *)obj)->field_2ed;
        obj->field_0b = obj->field_158;
    }
    return r;
}

/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int func_801b0e10_slot04_08(Object *obj) {
    if (func_801417cc(obj) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_0b = obj->field_158;
        if (((obj->field_134 | obj->field_136) & 0x6a) != 0) {
            obj->field_15a = 9;
            func_80142ba0(obj);
        } else {
            obj->field_15a = 2;
            func_80142b3c(obj);
        }
    }
}
