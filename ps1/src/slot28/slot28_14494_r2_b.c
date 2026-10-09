/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8004ab40_slot28[])(Object *);

void func_8002459c_slot28(Object *object) {
    data_8004ab40_slot28[object->field_04](object);
}
