/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_80142b3c(Object *object);

/* functions of other units of this module */

int func_801b0a04_slot04_11(Object *obj);

int func_801b0a04_slot04_11(Object *obj) {
    if (obj->kind != 0x11) return 0;
    if (func_801417cc(obj)) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 0;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
        return 1;
    } else {
        return 0;
    }
}
