/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0f44_slot04_03(Object *obj);


/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int func_801b0da4_slot04_03(Object *obj) {
    if ((s16)obj->field_c6 >= 0x30 && (u8)func_80141788(obj)) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 2;
        obj->field_12c = 0;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142778(obj);
    }
}

/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int func_801b0e24_slot04_03(Object *obj) {
    if ((s16)obj->field_c6 >= 0x30 && (u8)func_80141788(obj)) {
        func_801b0f44_slot04_03(obj);
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 3;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142778(obj);
    }
}

/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int func_801b0eac_slot04_03(Object *obj) {
    if ((s16)obj->field_c6 >= 0x30 && obj->field_240 == 0 && (u8)func_80141788(obj)) {
        func_801b0f44_slot04_03(obj);
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 4;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142718(obj);
    }
}

