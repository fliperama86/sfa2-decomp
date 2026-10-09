/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c60c0_slot04_0e[];

void func_801b4350_slot04_0e(Object *obj) {
    data_801c60c0_slot04_0e[obj->field_12c](obj);
}
