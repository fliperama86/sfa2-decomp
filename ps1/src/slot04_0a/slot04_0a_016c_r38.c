/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c0924_slot04_0a[])(Object *);

void func_801b4ff0_slot04_0a(Object *obj) {
}

void func_801b4ff8_slot04_0a(Object *obj) {
}

void func_801b5000_slot04_0a(Object *obj) {
}

void func_801b5008_slot04_0a(Object *obj) {
    data_801c0924_slot04_0a[obj->field_04](obj);
}
