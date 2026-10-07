/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800efa20_slot0f[])(Object *);

void func_800e6038_slot0f(Object *object) {
    data_800efa20_slot0f[object->field_04](object);
}
