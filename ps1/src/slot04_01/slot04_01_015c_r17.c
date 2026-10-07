/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801bf254_slot04_01[])(Object *);
extern ObjectFn data_801bf268_slot04_01[];

void func_801b3cfc_slot04_01(Object *obj) {
    data_801bf254_slot04_01[obj->field_05](obj);
}

void func_801b3d3c_slot04_01(Object *o) {
    data_801bf268_slot04_01[o->field_05](o);
    func_8011ffdc(o);
}
