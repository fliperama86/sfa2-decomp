/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800353c4_slot28[])(Object *);

void func_800193c8_slot28(Object *object) {
    data_800353c4_slot28[object->field_05](object);
}
