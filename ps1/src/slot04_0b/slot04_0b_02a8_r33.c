/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c2ff8_slot04_0b[])(Object *);

void func_801b4778_slot04_0b(Object *obj) {
    data_801c2ff8_slot04_0b[obj->field_05](obj);
}
