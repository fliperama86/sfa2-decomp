/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2994_slot04_07(Object *obj);

void func_801b293c_slot04_07(Object *obj) {
    update_sequence(obj);
    func_801b2994_slot04_07(obj);
    obj->other->other_state_code = 5;
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    }
}

void func_801b2994_slot04_07(Object *obj) {
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
}
