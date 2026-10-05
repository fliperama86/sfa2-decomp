/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801209c4(Object *o) {
    int a, c;
    if (o->field_293 != 0) {
        c = 0x32b;
        a = o->side;
    } else if (((0x10d40 >> o->kind) & 1) != 0) {
        c = 0x315;
        a = o->side;
    } else {
        a = o->side;
        c = 0x314;
    }
    func_80120554(o, a, c);
}
