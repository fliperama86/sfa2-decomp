/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6c40_slot04_02(Object *obj);

void func_801b6bf4_slot04_02(Object *obj) {
    obj->vel_y = -0x4c000;
    obj->state_3 = obj->state_3 + 1;
    if (obj->field_0b != 0) {
        obj->vel_x = 0x40000;
    } else {
        obj->vel_x = -0x40000;
    }
    func_801b6c40_slot04_02(obj);
}

void func_801b6c40_slot04_02(Object *obj) {
    s16 t;

    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    t = obj->field_70;
    if (t <= obj->pos_y) {
        obj->pos_y = t;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b6cb0_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d) {
    obj->state_0 = a;
    obj->state_1 = b;
    obj->state_2 = c;
    obj->state_3 = d;
}
