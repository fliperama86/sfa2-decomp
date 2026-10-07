/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801e9f50_slot06_00[])(Object *);

void func_801e8d84_slot06_00(Object *object) {
    data_801e9f50_slot06_00[object->field_04](object);
}
