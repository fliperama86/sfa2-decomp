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
    obj->state_3 = obj->state_3 + 1;
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
    obj->vel_y = 0x50000;
    obj->accel_y = -0x4000;
    obj->vel_x = k;
    obj->accel_x = 0;
    obj->field_45 = 1;
}

void func_801b03d0_slot04_05(Object *obj) {
    if (*(u8 *)&obj->step_flags == 0) {
        obj->state_3 = obj->state_3 + 1;
    }
    update_sequence(obj);
}

void func_801b0410_slot04_05(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->field_45 = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = (u16)obj->ground_y;
    }
    if (*(u8 *)&obj->step_flags != 2) {
        update_sequence(obj);
    }
}

void func_801b0474_slot04_05(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        game_state.field_4b |= 1 << obj->side;
    }
    update_sequence(obj);
}
