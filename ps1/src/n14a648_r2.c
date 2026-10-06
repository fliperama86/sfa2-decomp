/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Residual (not exact): the original loads the constant 1 into $v1 right after
   the lhu (hoisted into its delay slot) and stores it later; here 1 lands in
   $v0 after the store of 3, leaving a nop (+4 bytes). Tried: local for 1,
   store reorderings, chained assignment. The second function is exact. */
void func_8014b0e4(Object *object) {
    u16 value = *data_80189460++;
    data_80189464 = value;
    object->field_21c = value;
    object->field_209 = 4;
    object->field_04 = object->field_208 = 1;
    object->field_05 = 0;
    object->field_06 = 2;
    object->field_07 = 0;
    object->field_21a = object->field_48 = 1;
}
