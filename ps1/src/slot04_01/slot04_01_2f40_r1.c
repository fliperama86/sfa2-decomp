/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2f40_slot04_01(Object *obj) {
    obj->state_3++;
    obj->pos_y_frac = 0;
    obj->active_flag_4 = 1;
    obj->field_29c = 2;
    obj->active_state = 1;
    if (obj->facing_side == 0) {
        *(s32 *)&obj->vel_x = -0x40000;
    } else {
        *(s32 *)&obj->vel_x = 0x40000;
    }
    func_801307e0(obj, 0x48);
}

void func_801b2f9c_slot04_01(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        obj->state_3++;
        if (obj->facing_side == 0) {
            obj->pos_x -= 0x14;
        } else {
            obj->pos_x += 0x14;
        }
        play_sound_at(obj, ((Slot04aObj *)obj)->side, 0x317);
        func_80131d68(obj);
    }
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
}

void func_801b3030_slot04_01(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        obj->vel_y = 0x30000;
        obj->accel_y = -0x6000;
        obj->state_3++;
        if (obj->facing_side == 0) {
            obj->vel_x = 0x38000;
        } else {
            obj->vel_x = -0x38000;
        }
    }
}
