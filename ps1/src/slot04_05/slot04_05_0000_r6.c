/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b079c_slot04_05(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if ((t & 0xff) == 0) {
            if (obj->facing_side != 0) {
                *(u32 *)&obj->pos_x_frac = *(u32 *)&obj->pos_x_frac + obj->vel_x;
            } else {
                *(u32 *)&obj->pos_x_frac = *(u32 *)&obj->pos_x_frac - obj->vel_x;
            }
            obj->vel_x = obj->vel_x + obj->accel_x;
            if (obj->vel_x < 0) {
                obj->vel_x = 0;
                obj->accel_x = 0;
            }
        }
        update_sequence(obj);
    }
}
