/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2f74_slot04_0b(Object *obj) {
    if (obj->field_4c >= 0) {
        *(s32 *)&obj->field_14 -= obj->field_50;
        obj->field_50 += obj->field_58;
        if (obj->field_0b == 0) {
            *(s32 *)&obj->field_10 -= obj->field_4c;
        } else {
            *(s32 *)&obj->field_10 += obj->field_4c;
        }
        obj->field_4c += obj->field_54;
    } else {
        obj->field_4c = 0;
        obj->field_54 = 0;
        obj->field_07++;
    }
    func_80130efc(obj);
}
