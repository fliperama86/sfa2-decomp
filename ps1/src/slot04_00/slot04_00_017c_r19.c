/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c0550_slot04_00[])(Object *);

void func_801b3164_slot04_00(Object *obj) {
    data_801c0550_slot04_00[obj->field_05](obj);
}
