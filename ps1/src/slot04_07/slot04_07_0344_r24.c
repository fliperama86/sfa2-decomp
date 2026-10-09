/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c2124_slot04_07[])(Object *);

void func_801b4e8c_slot04_07(Object *obj) {
    data_801c2124_slot04_07[obj->field_04](obj);
}
