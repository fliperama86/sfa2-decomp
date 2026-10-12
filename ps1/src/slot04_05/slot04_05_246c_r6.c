/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b3048_slot04_05(Object *object);

void func_801b2c84_slot04_05(Object *obj) {
    s16 t = obj->step_flags;
    if (t & 0x8000) {
        obj->state_3++;
        if (obj->field_0b != 0) {
            obj->vel_x = 0x40000;
        } else {
            obj->vel_x = 0xfffc0000;
        }
        obj->vel_y = 0x90000;
        obj->accel_y = -0x7000;
        obj->accel_x = 0;
        obj->field_45 = 1;
        func_801307e0(obj, 0x2e);
    } else {
        switch ((u8)t) {
        case 0:
            break;
        default:
            if (obj->vel_x >= 0) {
                func_801b3048_slot04_05(obj);
            }
            break;
        case 3:
            obj->vel_x = 0x40000;
            obj->accel_x = 0xffff0000;
            break;
        }
        update_sequence(obj);
    }
}

void func_801b2d58_slot04_05(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        if ((u8)obj->step_flags != 1) {
            update_sequence(obj);
        }
    } else {
        obj->state_3++;
        obj->field_45 = 0;
        obj->pos_y = obj->ground_y;
    }
}
