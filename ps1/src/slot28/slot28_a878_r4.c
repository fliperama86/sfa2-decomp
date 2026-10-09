/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80037478_slot28[])(Object *);

void func_8001ad08_slot28(Object *obj) {
    data_80037478_slot28[obj->field_05](obj);
    obj->field_01 = 1;
}
