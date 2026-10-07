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
        t = obj->field_70;
        if (obj->pos_y >= t) {
            obj->pos_y = t;
            func_801209c4(obj);
            a = 0x37;
            s = obj->field_48;
            obj->field_14 = 0;
            obj->field_45 = 0;
            obj->field_07 = 8;
            obj->field_17b = 0;
            if (!(s & 0x80)) {
                a = (s & 0x7f) + 0x38;
            }
            func_801307e0(obj, a);
        }
    }
    if (obj->field_67 != 0) {
        a = obj->field_48 + 0x35;
        obj->field_67 = 0;
        obj->field_48 |= 0x80;
        func_801307e0(obj, a);
    } else {
        func_80130efc(obj);
    }
}
