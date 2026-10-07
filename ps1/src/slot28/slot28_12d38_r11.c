/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8004ab1c_slot28[])(Object *);

void func_80023fa0_slot28(Object *obj) {
}

void func_80023fa8_slot28(Object *object) {
    data_8004ab1c_slot28[object->field_05](object);
}
