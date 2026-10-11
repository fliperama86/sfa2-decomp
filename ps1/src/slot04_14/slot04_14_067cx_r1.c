/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b067c_slot04_14(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        obj->field_45 = 1;
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->field_70 <= obj->pos_y) {
            obj->state_3 = obj->state_3 + 1;
            obj->field_159 = 0;
            obj->field_45 = 0;
            obj->pos_y = (u16)obj->field_70;
            func_801209c4(obj);
            func_801307e0(obj, 0x41);
            return;
        }
    }
    update_sequence(obj);
}
