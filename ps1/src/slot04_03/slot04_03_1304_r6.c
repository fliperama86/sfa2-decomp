/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1938_slot04_03(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        obj->step_flags &= 0xff00;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, 0xf, 0x4e);
    } else {
        update_sequence(obj);
    }
}

void func_801b19d0_slot04_03(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        obj->step_flags &= 0xff00;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
        }
        obj->field_27b = 0;
    }
}

void func_801b1a3c_slot04_03(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->step_flags &= 0xff00;
        if (obj->field_0b != 0) {
            obj->vel_x = 0x44000;
        } else {
            obj->vel_x = -0x44000;
        }
    }
}
