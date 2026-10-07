/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c3e0c_slot04_08[])(Object *, Object *);

void func_801b4be0_slot04_08(Object *o, Object *p) {
    ModObj *obj = (ModObj *)o;
    data_801c3e0c_slot04_08[obj->field_03](o, p);
}
