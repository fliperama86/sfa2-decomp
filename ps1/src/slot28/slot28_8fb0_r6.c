/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800353d4_slot28[])(Object *);

void func_800194c8_slot28(Object *obj) {
}

void func_800194d0_slot28(Object *object) {
    data_800353d4_slot28[object->field_05](object);
}
