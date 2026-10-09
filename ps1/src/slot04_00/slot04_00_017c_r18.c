/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c0540_slot04_00[])(Object *);

void func_801b2f4c_slot04_00(Object *obj) {
    data_801c0540_slot04_00[obj->field_04](obj);
}
