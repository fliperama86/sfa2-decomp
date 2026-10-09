/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7a10_slot04_11[];
extern ObjectFn data_801c7a1c_slot04_11[];
extern ObjectFn data_801c7a24_slot04_11[];

void func_80142a14(Object *object);

void func_801b5f64_slot04_11(Object *obj);
void func_801b5f88_slot04_11(Object *obj);
int func_801b5fc8_slot04_11(Object *obj);
void func_801b604c_slot04_11(Object *obj);

void func_801b5b3c_slot04_11(Object *obj) {
    data_801c7a10_slot04_11[obj->field_128 >> 1](obj);
}

void func_801b5b80_slot04_11(Object *obj) {
    obj->field_157 = 0;
    data_801c7a1c_slot04_11[obj->field_07](obj);
}

void func_801b5bc0_slot04_11(Object *obj) {
    obj->field_07++;
    if (obj->field_12a == 0) {
        func_801b5f64_slot04_11(obj);
    } else if (obj->field_25f == 0 && (obj->field_130 & 0xa000) && (func_8013f8c4(obj, -0x14, 0xe) & 0xff)) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        func_801b5f64_slot04_11(obj);
        if (obj->field_129 == 0 && obj->field_12a != 0) {
            obj->field_278 = 1;
            obj->field_29a = 1;
        }
        func_801b5f88_slot04_11(obj);
    }
}

void func_801b5c94_slot04_11(Object *obj) {
    func_80142a14(obj);
    func_801b5f88_slot04_11(obj);
}

void func_801b5cc4_slot04_11(Object *obj) {
    obj->field_157 = 1;
    data_801c7a24_slot04_11[obj->field_07](obj);
}

void func_801b5d08_slot04_11(Object *obj) {
    obj->field_07++;
    func_801b5f64_slot04_11(obj);
    if (obj->field_129 != 0) {
        if (obj->field_12a == 4) {
            obj->field_278 = 1;
            func_801b5f88_slot04_11(obj);
        }
    } else if (obj->field_12a == 4) {
        obj->field_29c = 1;
        obj->field_29a = 1;
    }
}

void func_801b5d8c_slot04_11(Object *obj) {
    func_80142a14(obj);
    func_801b5f88_slot04_11(obj);
}

void func_801b5dbc_slot04_11(Object *obj) {
    if (obj->field_67 != 0 && *(u8 *)&obj->field_3a != 0 && (obj->field_134 & 8)) {
        func_801307e0(obj, obj->field_48 != 0 ? 0x19 : 0x18);
    } else if (func_801b5fc8_slot04_11(obj) < 0 && obj->pos_y >= obj->field_70) {
        func_801b604c_slot04_11(obj);
        func_801209c4(obj);
    } else {
        func_80130efc(obj);
    }
}
