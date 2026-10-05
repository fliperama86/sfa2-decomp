/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: RESIDUAL func_8012b024: original hoists "move a0,s0" above the first beq and uses a0 in the negative arm; built keeps it in the bltz slot (+4 bytes). Nested-if and merged-call forms tried. */



u8 func_80130184(Object *object);
void func_80130678(Object *object, int index);

void func_8012b0e4(Object *object) {
    object->field_04 = 1;
    object->field_05 = 0;
    object->field_06 = 3;
    object->field_07 = 0;
    object->field_242 = 0;
    object->field_0b = object->field_158;
    object->field_48 = object->field_21a;
    func_80130088(object);
    func_80130678(object, 0x10);
}

void func_8012b144(Object *object) {
    object->field_16a = 0;
    handler_table_19d0[object->field_07](object);
}
