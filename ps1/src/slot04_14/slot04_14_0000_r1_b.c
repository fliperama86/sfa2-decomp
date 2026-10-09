/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c61a0_slot04_14[];
extern ObjectFn data_801c61a8_slot04_14[];

void func_80130dc0(Object *object);
void func_801b62a8_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_801b0998_slot04_14(Object *obj);

void func_801b0734_slot04_14(Object *obj) {
    s16 t = obj->field_3a;

    if ((t & 0x8000) != 0) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b0778_slot04_14(Object *obj) {
    obj->field_157 = 1;
    data_801c61a0_slot04_14[obj->field_07](obj);
}

void func_801b07bc_slot04_14(Object *obj) {
    int one = 1;

    obj->field_159 = one;
    obj->field_07 = one;
    func_80130dc0(obj);
}

void func_801b07e4_slot04_14(Object *obj) {
    u16 x;

    obj->field_128 = 4;
    obj->field_07 = 3;
    obj->field_159 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->field_12a >> 1);
    x = 0xc;
    if (obj->field_48 != 0) {
        x = 0x12;
    }
    if (obj->field_129 != 0) {
        x += 3;
    }
    x += obj->field_12a >> 1;
    func_801307e0(obj, x);
    if ((u16)obj->pos_y < (u16)(obj->field_70 - 0x50) && obj->field_48 != 0 && (obj->field_48 & 0x80) == 0 && obj->field_129 != 0 && obj->field_12a == 2 && (obj->field_130 & 0x4000) != 0) {
        func_801b62a8_slot04_14(obj, 1, 0, 5, 0);
        func_801307e0(obj, 0x29);
    }
}

void func_801b0908_slot04_14(Object *obj) {
    data_801c61a8_slot04_14[obj->field_07](obj);
}

void func_801b0948_slot04_14(Object *obj) {
    obj->field_50 = -0x4c000;
    obj->field_4c = -0x40000;
    obj->field_07 = obj->field_07 + 1;
    if (obj->field_0b != 0) {
        obj->field_4c = 0x40000;
    }
    func_801b0998_slot04_14(obj);
}
