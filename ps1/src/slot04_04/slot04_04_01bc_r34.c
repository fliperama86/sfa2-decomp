/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b30c4_slot04_04(Object *o) {
    if (((s16)o->field_3a & 0x8000) == 0) {
        func_80130efc(o);
    } else {
        ref_other.p = o->other;
        ref_other.p->field_249 = 5;
        func_801312b8(o);
    }
}
