/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2994_slot04_07(Object *obj);

void func_801b248c_slot04_07(Object *obj) {
    int t;

    t = (s16)obj->ground_y;
    if (t <= obj->pos_y) {
        obj->state_3 = 5;
        obj->pos_y = t;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        func_801209c4(obj);
        func_801307e0(obj, 0x39);
    } else {
        if (*(u8 *)&obj->step_flags != 0) {
            update_sequence(obj);
            return;
        }
        func_801b2994_slot04_07(obj);
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
        obj->vel_y = obj->vel_y + obj->accel_y;
        update_sequence(obj);
    }
}
