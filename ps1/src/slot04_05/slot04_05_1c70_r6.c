/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c1528_slot04_05[];

void func_801b22e0_slot04_05(Object *obj) {
    u16 i = 0;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags == 0) {
        obj->state_3++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801c1528_slot04_05[i];
    }
}

void func_801b2370_slot04_05(Object *obj) {
    if (*(u8 *)&obj->step_flags == 1) {
        obj->active_state = 1;
        obj->state_3++;
    }
    update_sequence(obj);
}

void func_801b23b0_slot04_05(Object *obj) {
    if (obj->vel_y < 0) {
        obj->accel_y = obj->accel_y - 0x2000;
    }
    if ((u8)func_80130184(obj) != 0) {
        if (*(u8 *)&obj->step_flags != 2) {
            update_sequence(obj);
        }
    } else {
        obj->state_3++;
        obj->pos_y = obj->ground_y;
        obj->active_state = 0;
        if (obj->facing_side != 0) {
            obj->vel_x = 0x70000;
        } else {
            obj->vel_x = -0x70000;
        }
        obj->vel_y = 0x30000;
        obj->accel_x = 0;
        obj->accel_y = -0x6000;
    }
}
