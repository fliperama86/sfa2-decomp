/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c3dfc_slot04_08[])(Object *, Object *);

void func_801b4a80_slot04_08(Object *obj) {
    data_801c3dfc_slot04_08[obj->field_04](obj, obj->field_3c);
}
