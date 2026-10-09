/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80146478(Object *object, u8 a, int dx, int dy);

u8 func_801b625c_slot04_02(Object *obj) {
    Object *o = obj->other;
    u8 r = 0;
    int x = obj->field_46 + 0x100;
    s16 s;
    s8 t;
    s16 d;
    x &= 0x3ff;
    obj->field_46 = x;
    if ((x & 0x300) == 0) {
        s = func_80151184() & 0x7f;
        t = func_80151184();
        d = (t & 0x3f) - 0x60;
        func_80146478(obj, 2, d, s);
        if ((func_80151184() & 1) == 0) {
            func_80120554(o, o->side, 0x306);
        } else {
            func_80120554(o, o->side, 0x30f);
        }
        r = 1;
    }
    return r;
}
