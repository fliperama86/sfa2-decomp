/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801bf230_slot04_01[])(Object *);

void func_801b3ba0_slot04_01(Object *obj) {
    data_801bf230_slot04_01[obj->field_04](obj);
}
