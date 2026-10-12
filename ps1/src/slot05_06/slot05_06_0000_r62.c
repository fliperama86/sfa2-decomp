/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd574_slot05_06[];

void func_801cd524_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801cd588_slot05_06(Object *obj) {
    data_801dd574_slot05_06[obj->state_3](obj);
}

void func_801cd5c8_slot05_06(Object *obj) {
    obj->state_3++;
    if (obj->hit_flag_a != 0) {
        if ((u8)func_8013f8c4(obj, -0x18, 0x14) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        }
    } else {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}

void func_801cd640_slot05_06(Object *obj) {
    int d = 0x18;

    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->facing_side == 0) {
            d = -0x18;
        }
        obj->pos_x = d + obj->pos_x;
    }
    update_sequence(obj);
}
