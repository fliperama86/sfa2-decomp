/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8003c2d4_slot28[])(Object *);
extern void (*data_8003c2e4_slot28[])(Object *);

void func_8001d7d8_slot28(Object *obj) {
    data_8003c2d4_slot28[obj->field_03](obj);
    obj->field_01 = 1;
}

void func_8001d82c_slot28(Object *object) {
    data_8003c2e4_slot28[object->field_05](object);
}
