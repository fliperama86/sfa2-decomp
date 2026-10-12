/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0acc_slot04_07(Object *obj) {
    update_sequence(obj);
    if (obj->step_flags & 0xff) {
        if (obj->facing_side == 0) {
            obj->pos_x -= 0x1c;
        } else {
            obj->pos_x += 0x1c;
        }
    }
    if ((s16)obj->step_flags & 0x8000) {
        ((Slot04aObj *)obj)->field_1ca = 0;
        func_801312b8(obj);
    }
}
