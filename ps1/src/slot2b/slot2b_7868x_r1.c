/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80077908_slot2b(Object *obj);

/* What it does (inferred, not an original name): sets field_07 to 3 and
   field_159 to 1. When field_219 is not 0 it calls func_80077908_slot2b
   and ends. Otherwise it takes half of field_12a, passes it to
   func_80141f28, and then calls func_801307e0 with that half plus a base,
   0xc when field_48 is 0 and 0x12 otherwise, plus 3 when field_129 is not
   0, truncated to 16 bits. */
void func_80077868_slot2b(Object *obj) {
    /* Stand-in, not recovered source: the local t, which holds the half and is then used again for the base, reproduces the original's choice of registers. Without it this function differs from the original in 5 instruction slots. */
    /* The figure is for a local of its own for the base, as int; as u16 it is 8. Inferred: the original keeps the half before the first call and the base after it in the same register (v1), and that register is not the second argument's, which a value set only after the first call would take. */
    u16 t;
    u16 half;

    obj->field_07 = 3;
    obj->field_159 = 1;
    if (obj->field_219 != 0) {
        func_80077908_slot2b(obj);
    } else {
        t = obj->field_12a >> 1;
        half = t;
        func_80141f28(obj, t);
        t = obj->field_48 == 0 ? 0xc : 0x12;
        if (obj->field_129 != 0) {
            t += 3;
        }
        func_801307e0(obj, (u16)(t + half));
    }
}
