/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The same form as func_801b1884_slot04_08, whose unit has the measured
   variants. The two functions differ in their addresses only. */
void func_801b1c28_slot04_08(Object *obj) {
    int below;
    ((Slot04aObj *)obj)->field_1c8--;
    if (((Slot04aObj *)obj)->field_1c8 == 0) {
        obj->field_17b = 0;
    }
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
    if (obj->field_0b == 0) {
        below = -obj->vel_x < 0;
        if (below) {
            obj->state_3++;
        }
    } else {
        below = obj->vel_x < 0;
        if (below) {
            obj->state_3++;
        }
    }
    func_80130efc(obj);
}
