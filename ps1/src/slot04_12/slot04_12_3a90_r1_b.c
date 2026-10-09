/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8011f38c(Object *o);
u8 func_8013f8c4(Object *obj, int a, int b);
u8 func_80149b80(Object *obj);

void func_801b4228_slot04_12(Object *obj);
void func_801b42d0_slot04_12(Object *obj);
void func_801b4310_slot04_12(Object *obj);
void func_801b4350_slot04_12(Object *obj);
void func_801b43d8_slot04_12(Object *obj);
void func_801b4440_slot04_12(Object *obj);
void func_801b4480_slot04_12(Object *obj);
void func_801b44dc_slot04_12(Object *obj);
void func_801b4544_slot04_12(Object *obj);
void func_801b4a24_slot04_12(Object *obj);
void func_801b4ba8_slot04_12(Object *obj);

void func_801b4228_slot04_12(Object *obj) {
    obj->field_04 = obj->field_04 + 1;
}

void func_801b423c_slot04_12(Object *obj) {
    Object *p = obj->field_3c;

    if (obj->field_03 == 0) {
        *(s32 *)&p->field_2c = 0;
    } else {
        p->field_28 = 0;
    }
    func_8011f38c(obj);
}

void func_801b4278_slot04_12(Object *obj) {
    if (obj->field_128 == 4) {
        func_801b4a24_slot04_12(obj);
    } else if (obj->field_128 != 0) {
        func_801b4544_slot04_12(obj);
    } else {
        func_801b42d0_slot04_12(obj);
    }
}

void func_801b42d0_slot04_12(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b4440_slot04_12(obj);
    } else {
        func_801b4310_slot04_12(obj);
    }
}

void func_801b4310_slot04_12(Object *obj) {
    if (obj->field_07 == 0) {
        func_801b4350_slot04_12(obj);
    } else {
        func_801b43d8_slot04_12(obj);
    }
}

void func_801b4350_slot04_12(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0 && func_8013f8c4(obj, -0x14, 0x14) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        func_801b4ba8_slot04_12(obj);
    }
}

void func_801b43d8_slot04_12(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b4440_slot04_12(Object *obj) {
    if (obj->field_07 == 0) {
        func_801b4480_slot04_12(obj);
    } else {
        func_801b44dc_slot04_12(obj);
    }
}
