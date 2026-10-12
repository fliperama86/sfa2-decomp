/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b63c4_slot04_09(Object *obj, Object *parent);

void func_801b6358_slot04_09(Object *obj, Object *parent) {
    obj->state_0++;
    obj->field_1c = parent->field_1c;
    obj->table_index = parent->kind;
    obj->field_0c = parent->field_0c;
    obj->image_row = parent->image_row;
    obj->field_0e = parent->field_0e;
    obj->variant_index = 0;
    obj->countdown = 0;
    func_801b63c4_slot04_09(obj, parent);
}
