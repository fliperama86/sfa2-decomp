/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0878_slot04_09(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if ((t & 0xff) != 0) {
            if (obj->facing_side == 0) {
                obj->pos_x -= 2;
            } else {
                obj->pos_x += 2;
            }
        }
        update_sequence(obj);
    }
}
