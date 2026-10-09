/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80156898(void *a, Object *object);

void func_801564f0(void *a, Object *o) {
    u8 x, y;
    o->field_11c = 0x41;
    o->field_11d = 0x60;
    o->field_11e = 0x60;
    o->field_11f = 0x20;
    o->field_b4 = 0x960;
    *(s16 *)&o->field_b2 = -0x1000;
    o->field_123 = 0;
    o->field_b0 = 0x3c;
    o->field_aa++;
    func_80156898(a, o);
    func_80156a20(a, o);
    x = o->field_120;
    y = o->field_121;
    if ((u8)(x + y) >= 12) {
        func_80156818(a, o);
    } else {
        if (y < x) {
            x = y;
        }
        o->field_122 = x;
    }
}
