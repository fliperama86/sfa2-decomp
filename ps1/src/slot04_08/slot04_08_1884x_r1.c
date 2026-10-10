/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Each arm puts its comparison into a local and tests the local. With the
   comparisons tested in place, this function differs from the original in 4
   instruction slots; with the local in one arm only, in 4; with one test of
   the local after both arms, in 2, and it is 4 bytes longer. */
void func_801b1884_slot04_08(Object *obj) {
    int below;
    ((Slot04aObj *)obj)->field_1c8--;
    if (((Slot04aObj *)obj)->field_1c8 == 0) {
        obj->field_17b = 0;
    }
    *(s32 *)&obj->field_10 += obj->field_4c;
    obj->field_4c += obj->field_54;
    if (obj->field_0b == 0) {
        below = -obj->field_4c < 0;
        if (below) {
            obj->field_07++;
        }
    } else {
        below = obj->field_4c < 0;
        if (below) {
            obj->field_07++;
        }
    }
    func_80130efc(obj);
}
