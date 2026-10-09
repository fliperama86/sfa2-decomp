/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c2f2c_slot04_0b[])(Object *, Object *);
void func_801b3f98_slot04_0b(Object *obj, Object *parent);

void func_801b3efc_slot04_0b(Object *obj) {
    data_801c2f2c_slot04_0b[obj->field_04](obj, obj->field_3c);
}

void func_801b3f3c_slot04_0b(Object *obj, Object *parent) {
    obj->field_04++;
    obj->field_1c = parent->field_1c;
    obj->field_03 = parent->kind;
    obj->field_0c = parent->field_0c;
    obj->field_0e = parent->field_0e;
    obj->field_48 = 0;
    func_801b3f98_slot04_0b(obj, parent);
}
