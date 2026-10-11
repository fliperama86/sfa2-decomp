/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1c70_slot04_05(Object *obj) {
    int x;
    int base = 0x1f;

    obj->field_17b = 1;
    obj->state_3++;
    func_80141f28(obj, 2);
    func_80138ae8(&game_state, obj);
    x = data_801aa5e4[0];
    if (obj->field_0b == 0) {
        x += 0x1800000;
    }
    x -= *(s32 *)&obj->pos_x_frac;
    obj->vel_x = x >> 5;
    obj->vel_y = 0x90000;
    obj->accel_x = 0;
    obj->accel_y = -0x6000;
    if (obj->field_49 != 0) {
        obj->vel_x <<= 2;
        base = 0x3b;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + base);
}

void func_801b1d34_slot04_05(Object *o) {
    func_80130efc(o);
    if (*(u8 *)&o->step_flags != 0) {
        o->field_45 = 1;
        o->state_3++;
        o->field_48 = 0xff;
    }
    ((Slot04aObj *)o)->field_a0 = data_801aa5e4[0];
}
