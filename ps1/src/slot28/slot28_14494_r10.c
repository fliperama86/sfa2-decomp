/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8004c734_slot28[])(Object *);

void func_800252fc_slot28(Object *object) {
    data_8004c734_slot28[object->field_05](object);
}
