/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c3008_slot04_0b[])(Object *);

void func_801b4a8c_slot04_0b(Object *obj) {
    data_801c3008_slot04_0b[obj->field_05](obj);
}
