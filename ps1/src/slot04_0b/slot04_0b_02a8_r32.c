/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c2fe4_slot04_0b[])(Object *);
extern void (*data_801c2ff0_slot04_0b[])(Object *);

void func_801b4638_slot04_0b(Object *o) {
    ModObj *obj = (ModObj *)o;
    data_801c2fe4_slot04_0b[obj->field_03](o);
}

void func_801b4678_slot04_0b(Object *obj) {
    data_801c2ff0_slot04_0b[obj->field_05](obj);
}
