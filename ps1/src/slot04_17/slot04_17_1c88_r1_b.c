/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b56e0_slot04_17(Object *obj);

void func_801b29b0_slot04_17(Object *obj) {
    s16 t;
    int a;
    u8 s;

    if (func_801b56e0_slot04_17(obj) < 0) {
        t = obj->ground_y;
        if (obj->pos_y >= t) {
            obj->pos_y = t;
            func_801209c4(obj);
            a = 0x37;
            s = obj->variant_index;
            obj->pos_y_frac = 0;
            obj->active_state = 0;
            obj->state_3 = 8;
            obj->active_flag_4 = 0;
            if (!(s & 0x80)) {
                a = (s & 0x7f) + 0x38;
            }
            func_801307e0(obj, a);
        }
    }
    if (obj->active_flag != 0) {
        a = obj->variant_index + 0x35;
        obj->active_flag = 0;
        obj->variant_index |= 0x80;
        func_801307e0(obj, a);
    } else {
        update_sequence(obj);
    }
}
