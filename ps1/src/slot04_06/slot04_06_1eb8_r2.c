/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c5380_slot04_06[];

void func_801b2048_slot04_06(Object *obj) {
    obj->field_4c = data_801c5380_slot04_06[obj->field_12a >> 1];
}
