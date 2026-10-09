/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80015770_slot01[];

void func_80012b50_slot01(Object *object) {
    data_80015770_slot01[object->field_04](object);
}
