/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b32dc_slot04_00(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        obj->field_00 = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
    }
}
