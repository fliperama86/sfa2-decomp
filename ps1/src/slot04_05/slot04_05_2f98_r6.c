/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3734_slot04_05(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->facing_side = 0;
    if ((obj->input_bits & 0x8000) != 0) {
        obj->facing_side = 1;
        obj->vel_x = -0x30000;
    } else {
        obj->vel_x = 0x30000;
    }
    obj->accel_x = 0;
    obj->vel_y = 0x40000;
    obj->accel_y = -0xe000;
    func_801307e0(obj, 0x19);
}

void func_801b37d0_slot04_05(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
    }
    update_sequence(obj);
}

void func_801b3810_slot04_05(Object *obj) {
    if (*(u8 *)&obj->step_flags == 2) {
        obj->active_state = 1;
        obj->state_3 = obj->state_3 + 1;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        update_sequence(obj);
    }
}

void func_801b3864_slot04_05(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
    }
    if (*(u8 *)&obj->step_flags != 3) {
        update_sequence(obj);
    }
}
