/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801c8670_slot05_06(Object *obj);
void func_801c8744_slot05_06(Object *obj);
extern ObjectFn data_801dd258_slot05_06[];

void func_801c8510_slot05_06(Object *obj) {
    int one;

    obj->state_3 = obj->state_3 + 1;
    if ((obj->field_130 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x18, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        one = 1;
        obj->field_159 = one;
        func_80130dc0(obj);
        obj->field_278 = one;
    }
}

void func_801c8598_slot05_06(Object *obj) {
    int d = -0x10;

    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_0b != 0) {
            d = 0x10;
        }
        obj->pos_x = d + obj->pos_x;
    }
    update_sequence(obj);
}

void func_801c85f0_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801c8630_slot05_06(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_801c8744_slot05_06(obj);
    } else {
        func_801c8670_slot05_06(obj);
    }
}

void func_801c8670_slot05_06(Object *obj) {
    data_801dd258_slot05_06[obj->field_12a >> 1](obj);
}
