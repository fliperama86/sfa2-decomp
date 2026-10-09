/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c09d8_slot04_03[])(Object *, Object *);

void func_801b3eac_slot04_03(Object *obj) {
    data_801c09d8_slot04_03[obj->field_04](obj, obj->field_3c);
}
