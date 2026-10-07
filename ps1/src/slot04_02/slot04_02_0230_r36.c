/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c7874_slot04_02[])(Object *);

void func_801b7974_slot04_02(Object *obj) {
    data_801c7874_slot04_02[obj->field_04](obj);
}
