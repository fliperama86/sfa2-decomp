/* Reconstruction. Names/roles inferred, not original symbols. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual (v14 retry): 244 vs 252 bytes. Original ends with if (flag) {...; a1 = 0xe} else a1 = s1 (state kept in $s1, 0xe loaded into $a1). Here the 0xc is constant-propagated or both paths share one register. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012e168(Object *o) {
    u16 state;
    if (func_8012eff8(o)) {
        o->field_07++;
        o->field_0b = (o->field_72 ^ 1) & 1;
        func_8012e334(o);
        return;
    }
    if ((s16)o->field_5c >= 0) {
        if (o->field_cd) {
            func_8012e264(o);
        } else if ((o->field_130 & 0x2000) == 0) {
            func_80130efc(o);
        } else {
            o->field_157 = 0;
            state = 0xc;
        }
    }
    if ((o->field_134 & 0x4000) == 0) {
        func_801308c4(o, state);
        return;
    }
    o->field_157++;
    o->field_29f = 0;
    o->field_60 = 0;
    state = 0xe;
    func_801308c4(o, state);
}
