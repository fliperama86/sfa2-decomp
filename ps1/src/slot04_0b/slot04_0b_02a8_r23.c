/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c2ee8_slot04_0b[])(Object *);

void func_801b39f0_slot04_0b(Object *obj, Object *unused) {
    data_801c2ee8_slot04_0b[obj->field_05](obj);
}
