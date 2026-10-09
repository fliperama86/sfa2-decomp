/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c55b4_slot04_06[])(Object *);
extern Object *data_801c5758_slot04_06;

void func_801b5b68_slot04_06(Object *obj) {
    data_801c5758_slot04_06 = obj->field_3c;
    data_801c55b4_slot04_06[obj->field_04](obj);
}
