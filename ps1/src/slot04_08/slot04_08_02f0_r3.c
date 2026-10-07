/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c3b68_slot04_08[])(Object *);

void func_801b05c4_slot04_08(Object *obj) {
    obj->field_157 = 1;
    data_801c3b68_slot04_08[obj->field_07](obj);
}
