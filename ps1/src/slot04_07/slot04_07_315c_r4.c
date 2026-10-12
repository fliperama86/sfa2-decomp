/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b35a8_slot04_07(Object *obj) {
    u16 w;
    int t;
    int a;
    int b;

    obj->hit_count_3 = obj->hit_count_3 + 1;
    update_sequence(obj);
    w = obj->countdown;
    t = (w & 0xff) - 1;
    a = (w & 0xff00) | t;
    obj->countdown = a;
    if (t == 0) {
        if (obj->active_state != 0) {
            obj->state_0 = 1;
        }
        obj->state_2 = 3;
        obj->state_1 = 0;
        obj->state_3 = 1;
        set_side_flag(obj);
        func_80142c70(obj);
    } else {
        if (obj->pressed_bits != 0) {
            if (((Slot04aObj *)obj)->field_27d == 0) {
                ((Slot04aObj *)obj)->field_27d = obj->hit_count_3;
            }
        } else {
            if ((s16)a != 0) {
                b = a - 1;
                obj->countdown = b;
                if ((b & 1) != 0) {
                    goto call;
                }
            }
            set_side_flag(obj);
        }
call:
        func_80142fe8(obj);
    }
}
