/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c3dd4_slot04_08[])(Object *, Object *);

void func_801b43bc_slot04_08(Object *obj) {
    data_801c3dd4_slot04_08[obj->field_04](obj, obj->field_3c);
}
