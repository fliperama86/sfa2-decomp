/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c4510_slot04_04[])(Object *);

void func_801b3b00_slot04_04(Object *obj) {
    data_801c4510_slot04_04[obj->field_04](obj);
}
