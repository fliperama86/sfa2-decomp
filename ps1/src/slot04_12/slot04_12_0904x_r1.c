/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Declared here because the published headers lack it (inferred). */
void func_801b09ac_slot04_12(Object *obj);

/* What it does (inferred, not an original name): a state entry of a character.
   When the object's field_12a is 2 and the top nibble of its field_130 is 4,
   it sets the four state bytes at 4..7, spends one point (func_80141f28 with
   1), clears field_12f and field_67 and starts sequence 0x1d (field_48 not 0)
   or 0x1c. Otherwise it hands over to the module's function
   func_801b09ac_slot04_12. */
void func_801b0904_slot04_12(Object *obj) {
    /* The call of func_801b09ac_slot04_12 stands in two arms, once for each test that leads to it. With one arm for both (`if (obj->field_12a == 2 && (obj->field_130 & 0xf000) == 0x4000) { ... } else { ... }`) this function differs from the original in 6 instruction slots. */
    if (obj->field_12a != 2) {
        func_801b09ac_slot04_12(obj);
    } else if ((obj->field_130 & 0xf000) == 0x4000) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 5;
        obj->state_3 = 0;
        func_80141f28(obj, 1);
        obj->field_12f = 0;
        obj->field_67 = 0;
        if (obj->field_48 != 0) {
            func_801307e0(obj, 0x1d);
        } else {
            func_801307e0(obj, 0x1c);
        }
    } else {
        func_801b09ac_slot04_12(obj);
    }
}
