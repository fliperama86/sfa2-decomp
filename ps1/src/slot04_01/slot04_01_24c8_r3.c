/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2898_slot04_01(Object *obj) {
    u8 t = obj->state_3;

    obj->active_flag_4 = 1;
    obj->field_29c = 2;
    obj->active_flag_3 = 1;
    obj->active_state = 1;
    t++;
    obj->state_3 = t;
    func_80138ae8(&game_state, obj);
    if (obj->facing_side != 0) {
        obj->vel_x = 0x40000;
    } else {
        obj->vel_x = -0x40000;
    }
    func_801307e0(obj, (obj->direction_index >> 1) + 0x35);
}

void func_801b291c_slot04_01(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->other->other_state_code = 5;
        func_80131468(obj);
    } else {
        if ((t & 0xff) == 0) {
            *(s32 *)&obj->pos_x_frac += obj->vel_x;
        }
        update_sequence(obj);
    }
}
