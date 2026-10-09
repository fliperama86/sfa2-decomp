/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c3dbc_slot04_08[])(Object *, Object *);

void func_801b3e34_slot04_08(Object *obj, Object *p) {
    data_801c3dbc_slot04_08[obj->field_07](obj, p);
}
