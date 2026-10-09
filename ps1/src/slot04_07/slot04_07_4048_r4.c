/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b45a8_slot04_07(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            if (obj->field_12a != 0) {
                if (obj->field_0b == 0) {
                    obj->pos_x = obj->pos_x - 0x18;
                } else {
                    obj->pos_x = obj->pos_x + 0x18;
                }
            }
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b464c_slot04_07(Object *obj) {
    s16 t;
    s16 d;
    int m;

    func_80130efc(obj);
    t = obj->field_3a;
    if ((t & 0x8000) == 0) {
        m = t & 0xff;
        d = m;
        if (m != 0) {
            if (obj->field_0b == 0) {
                d = -m;
            }
            obj->pos_x = d + obj->pos_x;
            obj->field_3a = obj->field_3a & 0xff00;
        }
    } else {
        func_801312b8(obj);
    }
}
