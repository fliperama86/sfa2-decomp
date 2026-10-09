/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8004ea68_slot28[])(Object *);

void func_80026254_slot28(Object *obj) {
}

void func_8002625c_slot28(Object *object) {
    data_8004ea68_slot28[object->field_05](object);
}
