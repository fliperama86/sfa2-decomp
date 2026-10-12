/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b05a4_slot04_02(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        obj->active_state = 1;
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->ground_y <= obj->pos_y) {
            obj->state_3 = obj->state_3 + 1;
            obj->active_flag_3 = 0;
            obj->active_state = 0;
            obj->pos_y = (u16)obj->ground_y;
            func_801209c4(obj);
            func_801307e0(obj, 0x41);
            return;
        }
    }
    update_sequence(obj);
}
