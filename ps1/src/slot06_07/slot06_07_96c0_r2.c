/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ee1ac_slot06_07[];

void func_801e9d84_slot06_07(Object *obj);

void func_801e9c70_slot06_07(Object *obj) {
    data_801ee1ac_slot06_07[obj->state_0](obj);
}

void func_801e9cb0_slot06_07(Object *obj) {
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->spawn_mode = 4;
    obj->state_0 = obj->state_0 + 1;
    func_801e9d84_slot06_07(obj);
}
