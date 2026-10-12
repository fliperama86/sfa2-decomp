/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b45a8_slot04_07(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            if (obj->direction_index != 0) {
                if (obj->facing_side == 0) {
                    obj->pos_x = obj->pos_x - 0x18;
                } else {
                    obj->pos_x = obj->pos_x + 0x18;
                }
            }
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b464c_slot04_07(Object *obj) {
    s16 t;
    s16 d;
    int m;

    update_sequence(obj);
    t = obj->step_flags;
    if ((t & 0x8000) == 0) {
        m = t & 0xff;
        d = m;
        if (m != 0) {
            if (obj->facing_side == 0) {
                d = -m;
            }
            obj->pos_x = d + obj->pos_x;
            obj->step_flags = obj->step_flags & 0xff00;
        }
    } else {
        func_801312b8(obj);
    }
}
