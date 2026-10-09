/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80037480_slot28[])(Object *);

void func_8001ae68_slot28(Object *object) {
    data_80037480_slot28[object->field_05](object);
}
