/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c4598_slot04_04[])(Object *);

void func_801b3d18_slot04_04(Object *o) {
    if ((s16)o->field_3a < 0) {
        o->field_04 = o->field_04 + 1;
    }
    func_80131094(o);
}

void func_801b3d58_slot04_04(Object *o) {
    Object *p = o->field_3c;
    p->field_240--;
    if (p->field_240 == 0) {
        p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)o);
}

void func_801b3da4_slot04_04(Object *obj) {
    data_801c4598_slot04_04[obj->field_04](obj);
}
