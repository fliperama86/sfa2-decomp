/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80035414_slot28[])(Object *);

void func_800198f8_slot28(Object *obj) {
    if (obj->field_48 != 0) {
        obj->field_04++;
    }
}

void func_80019920_slot28(Object *object) {
    data_80035414_slot28[object->field_05](object);
}
