/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2eb4_slot04_08(Object *obj) {
    if (((Slot04aObj *)obj)->sub_index != 2) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        obj->vel_x = obj->vel_x + obj->accel_x;
    }
    if ((s16)obj->step_flags & 0x8000) {
        ((Slot04aObj *)obj)->sub_index++;
        if (((Slot04aObj *)obj)->sub_index == 3) {
            func_801312b8(obj);
        } else {
            obj->state_3 = 1;
            func_801307e0(obj, (obj->direction_index >> 1) + 0x43);
        }
    } else {
        update_sequence(obj);
    }
}
