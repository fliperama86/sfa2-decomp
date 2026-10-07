/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c0994_slot04_03[])(Object *, Object *);

void func_801b3aa0_slot04_03(Object *obj) {
}

/* The second parameter is passed on unread: entries of the table read the second argument register, and this function does not set it. What the caller outside the module passes there is not known from this module. */
void func_801b3aa8_slot04_03(Object *obj, Object *p) {
    data_801c0994_slot04_03[obj->field_04](obj, p);
}
