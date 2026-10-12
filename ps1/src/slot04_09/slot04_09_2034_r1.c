/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c7cb0_slot04_09[];
void func_801b27f8_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);

void func_801b2034_slot04_09(Object *obj) {
    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    if (obj->vel_y >= 0 && (s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        func_801307e0(obj, 0x22);
    }
}

void func_801b20ac_slot04_09(Object *obj) {
    Object *o;
    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        o = obj->other;
        o->field_15b = 1;
        o->hit_flag = 1;
        func_80140770(obj, 5, 10, *(s16 *)((u8 *)data_801c7cb0_slot04_09 + (obj->direction_index & 0xfe)), 0, 1, 0);
        o = obj->other;
        if ((s16)o->health < 0) {
            obj->field_167 = 2;
        }
        obj->vel_x = 0x20000;
        obj->vel_y = 0x40000;
        obj->accel_x = 0;
        obj->accel_y = -0x4000;
        if (obj->facing_side == 0) {
            obj->vel_x = 0xfffe0000;
        }
    }
}
