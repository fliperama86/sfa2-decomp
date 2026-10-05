/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Historical note, from before this unit was exact or about a function that is no longer in this unit: NOT EXACT (200 vs 208 bytes). Residual: the original keeps two separate
 * 'a1 = 0xc; j' blocks (for field_29f and for field_157 == 0); the compiler
 * cross-jumps them into one. Tried explicit calls, goto, ternary, merged tests. */
void func_8012e264(Object *o) {
    int state;
    if ((s16)o->field_c6 >= 0x30 && (s16)o->field_5c >= 0 && o->field_221) {
        func_801303a0(o);
        return;
    }
    o->field_157 = 0;
    if (o->field_29f) {
        func_801308c4(o, 0xc);
        return;
    }
    ref_other.p = o->other;
    if (ref_other.p->field_45 != 0) {
        func_801308c4(o, 0xc);
        return;
    }
    if (ref_other.p->field_157 == 0) {
        func_801308c4(o, 0xc);
        return;
    }
    o->field_157++;
    func_801308c4(o, 0xe);
}
