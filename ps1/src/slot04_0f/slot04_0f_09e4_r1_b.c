/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013d210(Object *object);
int func_8013d1a8(Object *object);
int func_8013cac8(Object *object, u8 a, u8 b);
int func_8013caf0(Object *object, u8 a, u8 b);
int func_8013cb70(Object *object, u8 a, u8 b);
u8 func_801417cc(Object *object);
u8 func_80141c4c(Object *object);

/* functions of other units of this module */
int func_801b132c_slot04_0f(Object *obj);
int func_801b14b0_slot04_0f(Object *obj);
int func_801b1418_slot04_0f(Object *obj);
int func_801b171c_slot04_0f(Object *obj);
int func_801b17c8_slot04_0f(Object *obj);
int func_801b1958_slot04_0f(Object *obj);
int func_801b19f4_slot04_0f(Object *obj);
int func_801b18c0_slot04_0f(Object *obj);
int func_801b1530_slot04_0f(Object *obj);
int func_801b1628_slot04_0f(Object *obj);

int func_801b11e4_slot04_0f(Object *obj);
int func_801b12bc_slot04_0f(Object *obj);

void func_801b0fb8_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_46--;
    if (obj->field_46 != 0) {
        func_801312b8(o);
    } else {
        func_80130efc(o);
    }
}

void func_801b1008_slot04_0f(Object *obj) {
    if (obj->field_45 == 0) {
        ((Slot04bObj *)obj)->field_334 = 0;
    }
    if (func_8013d210(obj) && func_801b11e4_slot04_0f(obj)) return;
    if (func_8013d1a8(obj) && func_801b132c_slot04_0f(obj)) return;
    if (func_8013caf0(obj, 0, 0x15) && func_801b14b0_slot04_0f(obj)) return;
    if (func_8013cb70(obj, 1, 0x18) && func_801b1418_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 2, 0x1b) && func_801b171c_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 3, 0x1c) && func_801b17c8_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 4, 0x12) && func_801b1958_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 5, 0x13) && func_801b19f4_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 6, 0) && func_801b18c0_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 7, 0xe) && func_801b12bc_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 8, 0xd) && func_801b1530_slot04_0f(obj)) return;
    if (func_8013cac8(obj, 9, 0xe)) func_801b1628_slot04_0f(obj);
}

int func_801b11e4_slot04_0f(Object *obj) {
    if (obj->field_7e == 0 && obj->field_177 == 0) {
        return 0;
    }
    if (obj->field_45 == 0) {
        if (func_801417cc(obj)) {
            obj->field_04 = 1;
            obj->field_06 = 7;
            obj->field_05 = 0;
            obj->field_07 = 0;
            obj->field_15a = 0xa;
            obj->field_0b = obj->field_158;
            return 1;
        }
        return 0;
    }
    if (obj->field_50 < 0) {
        return 0;
    }
    if (func_801418bc(obj)) {
        obj->field_04 = 1;
        obj->field_06 = 7;
        obj->field_05 = 0;
        obj->field_07 = 0;
        obj->field_15a = 0xb;
        obj->field_0b = obj->field_158;
        return 1;
    }
    return 0;
}

/* The call of func_80141c4c passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 4 instruction slots. */
int func_801b12bc_slot04_0f(Object *obj) {
    if (obj->field_260 != 0) {
        return 0;
    }
    if (((int (*)(void))func_80141c4c)()) {
        obj->field_04 = 1;
        obj->field_06 = 7;
        obj->field_05 = 0;
        obj->field_07 = 0;
        obj->field_15a = 6;
        obj->field_159 = 0;
        return 1;
    }
    return 0;
}
