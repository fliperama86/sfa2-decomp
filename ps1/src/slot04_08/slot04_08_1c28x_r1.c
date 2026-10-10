/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The same form as func_801b1884_slot04_08, whose unit has the measured
   variants. The two functions differ in their addresses only. */
void func_801b1c28_slot04_08(Object *obj) {
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
