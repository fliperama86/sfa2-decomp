/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80130184(Object *object);

void func_801b39ec_slot04_02(Object *obj) {
    int n;

    if ((u8)func_80130184(obj) == 0) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        n = 0x39;
        if (obj->field_48 == 1) {
            n = 0x38;
        }
        func_801307e0(obj, n);
    } else {
        func_80130efc(obj);
    }
}
