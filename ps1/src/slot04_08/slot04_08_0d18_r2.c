/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80141788(Object *object);
void func_80142718(Object *object);
void func_80142778(Object *object);

/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int func_801b0ea0_slot04_08(Object *obj) {
    if ((s16)obj->field_c6 >= 0x30 && func_80141788(obj) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 3;
        obj->field_159 = 1;
        obj->field_12c = 0;
        obj->field_0b = obj->field_158;
        if (obj->field_25c != 0) {
            if (((obj->field_134 | obj->field_136) & 0x30) != 0) {
                obj->field_129 = 0;
                func_80142718(obj);
            } else {
                obj->field_129 = 2;
                func_80142778(obj);
            }
        } else {
            if (((obj->field_134 | obj->field_136) & 0x6a) != 0) {
                obj->field_129 = 2;
                func_80142778(obj);
            } else {
                obj->field_129 = 0;
                func_80142718(obj);
            }
        }
    }
}

/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int func_801b0f80_slot04_08(Object *obj) {
    if ((s16)obj->field_c6 >= 0x30 && func_80141788(obj) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 4;
        obj->field_159 = 1;
        obj->field_12c = 0;
        obj->field_0b = obj->field_158;
        func_80142718(obj);
    }
}
