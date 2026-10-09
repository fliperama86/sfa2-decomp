/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ec944_slot06_09[];
extern SequenceStep *data_801ec92c_slot06_09[];

void func_801e9cc0_slot06_09(Object *obj);

void func_801e9b18_slot06_09(Object *obj) {
    data_801ec944_slot06_09[obj->field_04](obj);
}

void func_801e9b58_slot06_09(Object *obj) {
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_81 = 4;
    obj->field_4c = 0;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_04++;
    func_801e9cc0_slot06_09(obj);
    func_80130700(obj, data_801ec92c_slot06_09[obj->field_03]);
}
