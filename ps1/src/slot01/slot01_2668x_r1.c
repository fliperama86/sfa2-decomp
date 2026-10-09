/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80015440_slot01[][0x60];

void func_80012668_slot01(Object *obj) {
    Rect r;
    u8 *p;
    Object *o = obj->field_3c;
    r.x = 0x70;
    r.y = 0x1ec;
    r.w = 0x10;
    r.h = 3;
    func_80157fc4(&r, data_80015440_slot01[o->field_d4]);
    p = data_801a2964;
    func_80158028(&r, p);
    p += 0x1400;
    func_80158028(&r, p);
    func_80157d9c(0);
}
