/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b2510_slot04_0b(Object *object);

void func_801b283c_slot04_0b(Object *obj) {
    int t;
    unsigned u;
    int y;

    if (func_801b2510_slot04_0b(obj)) {
        if ((u8)obj->field_3a != 4) {
            func_80130efc(obj);
        }
    } else {
        t = obj->field_07;
        u = obj->field_12a;
        y = ((Slot04aObj *)obj)->field_70;
        obj->field_45 = 0;
        obj->field_a0 = 0;
        t++;
        obj->field_07 = t;
        obj->pos_y = y;
        func_801307e0(obj, (u >> 1) + 0x45);
    }
}
