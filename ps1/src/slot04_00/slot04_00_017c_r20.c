/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80137cc0(Object *object);
void func_80137dc8(Object *object);
void func_80137f00(Object *object);
extern ObjectFn data_801c056c_slot04_00[];

void func_801b3314_slot04_00(Object *o) {
    func_80137cc0(o);
}

void func_801b3334_slot04_00(Object *obj) {
    func_80137dc8(obj);
}

void func_801b3354_slot04_00(Object *obj) {
    func_80137f00(obj);
}

void func_801b3374_slot04_00(Object *o) {
    data_801c056c_slot04_00[o->field_05](o);
    func_8011ffdc(o);
}
