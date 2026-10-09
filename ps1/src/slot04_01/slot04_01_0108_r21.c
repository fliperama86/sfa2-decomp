/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801bf270_slot04_01[])(Object *, Object *);
void func_801b3eec_slot04_01(Object *obj, Object *parent);

void func_801b3dc4_slot04_01(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_04 = 3;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
    }
    func_80131094(obj);
}

void func_801b3e08_slot04_01(Object *o) {
    Object *p = o->field_3c;
    u8 n = p->field_240 - 1;
    p->field_240 = n;
    if (n == 0) {
        p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)o);
}

void func_801b3e50_slot04_01(Object *obj) {
    data_801bf270_slot04_01[obj->field_04](obj, obj->field_3c);
}

void func_801b3e90_slot04_01(Object *obj, Object *parent) {
    obj->field_04++;
    obj->field_1c = parent->field_1c;
    obj->field_03 = parent->kind;
    obj->field_0c = parent->field_0c;
    obj->field_0e = parent->field_0e;
    obj->field_48 = 0;
    func_801b3eec_slot04_01(obj, parent);
}
