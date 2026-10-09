/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c0500_slot04_03[];

void func_801b00fc_slot04_03(Object *obj) {
    data_801c0500_slot04_03[obj->field_06](obj);
}
