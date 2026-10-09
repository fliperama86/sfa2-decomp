/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8004ab04_slot28[])(Object *);
extern void (*data_8004ab14_slot28[])(Object *);

void func_80023e9c_slot28(Object *obj) {
    data_8004ab04_slot28[obj->field_03](obj);
    obj->field_01 = 1;
}

void func_80023ef0_slot28(Object *object) {
    data_8004ab14_slot28[object->field_05](object);
}
