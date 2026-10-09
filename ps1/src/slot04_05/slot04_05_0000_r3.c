/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b033c_slot04_05(Object *obj) {
    int e;
    s16 d;
    int base;
    int k;

    k = -0x10000;
    obj->field_07 = obj->field_07 + 1;
    base = box_margin[0];
    if (obj->field_0b == 0) {
        base += 0x180;
    }
    e = base - (u16)obj->pos_x;
    d = e;
    if (obj->field_0b != 0) {
        d = -e;
    }
    if (d < 0x30) {
        obj->field_0b = obj->field_0b ^ 1;
    }
    if (obj->field_0b == 0) {
        k = 0x10000;
    }
    obj->field_50 = 0x50000;
    obj->field_58 = -0x4000;
    obj->field_4c = k;
    obj->field_54 = 0;
    obj->field_45 = 1;
}

void func_801b03d0_slot04_05(Object *obj) {
    if (*(u8 *)&obj->field_3a == 0) {
        obj->field_07 = obj->field_07 + 1;
    }
    func_80130efc(obj);
}

void func_801b0410_slot04_05(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->field_45 = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = (u16)obj->field_70;
    }
    if (*(u8 *)&obj->field_3a != 2) {
        func_80130efc(obj);
    }
}

void func_801b0474_slot04_05(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        game_state.field_4b |= 1 << obj->side;
    }
    func_80130efc(obj);
}
