/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c65cc_slot04_02[])(Object *);

void func_801b6cc8_slot04_02(Object *obj) {
    data_801c65cc_slot04_02[obj->field_04](obj);
}
