/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c4660_slot04_04[])(Object *);

void func_801b4c2c_slot04_04(Object *obj) {
    data_801c4660_slot04_04[obj->field_04](obj);
}
