/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80048cbc_slot28[])(Object *);

void func_80022e0c_slot28(Object *object) {
    data_80048cbc_slot28[object->field_04](object);
}
