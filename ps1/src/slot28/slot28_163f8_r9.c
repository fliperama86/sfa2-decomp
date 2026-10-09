/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800517f0_slot28[])(Object *);

void func_80028280_slot28(Object *object) {
    data_800517f0_slot28[object->field_04](object);
}
