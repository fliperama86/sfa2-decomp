/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c7e14_slot04_09[])(Object *, Object *);

void func_801b6318_slot04_09(Object *obj) {
    data_801c7e14_slot04_09[obj->field_04](obj, obj->field_3c);
}
