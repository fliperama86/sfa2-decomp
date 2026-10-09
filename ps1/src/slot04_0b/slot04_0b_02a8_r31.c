/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c2fd4_slot04_0b[])(Object *);

void func_801b44fc_slot04_0b(Object *obj) {
    ref_other.p = obj->field_3c;
    data_801c2fd4_slot04_0b[obj->field_04](obj);
}
