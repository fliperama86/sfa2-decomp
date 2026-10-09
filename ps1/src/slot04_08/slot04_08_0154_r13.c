/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c3db4_slot04_08[])(Object *, Object *);

/* The second parameter is passed on unread: entries of the table read the second argument register, and this function does not set it. What the caller outside the module passes there is not known from this module. */
void func_801b3df0_slot04_08(Object *obj, Object *p) {
    data_801c3db4_slot04_08[obj->field_129 >> 1](obj, p);
}
