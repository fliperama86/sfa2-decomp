/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0764_slot04_00(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        obj->active_state = 1;
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->pos_y >= obj->ground_y) {
            obj->state_3 = obj->state_3 + 1;
            obj->active_state = 0;
            obj->active_flag_3 = 0;
            obj->pos_y = (u16)obj->ground_y;
            func_801209c4(obj);
            func_801307e0(obj, 0x30);
            return;
        }
    }
    update_sequence(obj);
}

void func_801b081c_slot04_00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->other_state_code = 6;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}
