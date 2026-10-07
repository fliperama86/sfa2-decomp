/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2460_slot04_0b(Object *o) {
    if ((s16)o->field_3a & 0x8000) {
        o->field_07++;
        o->field_45 = 0;
        func_801209c4(o);
        func_801307e0(o, 0x30);
    } else {
        func_80130efc(o);
    }
}
