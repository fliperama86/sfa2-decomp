/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b35a8_slot04_07(Object *obj) {
    u16 w;
    int t;
    int a;
    int b;

    obj->field_27c = obj->field_27c + 1;
    update_sequence(obj);
    w = obj->field_46;
    t = (w & 0xff) - 1;
    a = (w & 0xff00) | t;
    obj->field_46 = a;
    if (t == 0) {
        if (obj->field_45 != 0) {
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
                ((Slot04aObj *)obj)->field_27d = obj->field_27c;
            }
        } else {
            if ((s16)a != 0) {
                b = a - 1;
                obj->field_46 = b;
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
