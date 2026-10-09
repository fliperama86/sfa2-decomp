/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8002c76c_slot28[])(Object *);

void func_800158dc_slot28(Object *object) {
    data_8002c76c_slot28[object->field_04](object);
}
