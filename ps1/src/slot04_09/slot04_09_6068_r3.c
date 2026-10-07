/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b63c4_slot04_09(Object *obj, Object *parent);

void func_801b6358_slot04_09(Object *obj, Object *parent) {
    obj->field_04++;
    obj->field_1c = parent->field_1c;
    obj->field_03 = parent->kind;
    obj->field_0c = parent->field_0c;
    obj->field_0d = parent->field_0d;
    obj->field_0e = parent->field_0e;
    obj->field_48 = 0;
    obj->field_46 = 0;
    func_801b63c4_slot04_09(obj, parent);
}
