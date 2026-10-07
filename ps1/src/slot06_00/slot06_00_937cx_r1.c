/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801e9f44_slot06_00[];

void func_801e937c_slot06_00(Object *object) {
    object->field_0a = 1;
    object->field_0f = 1;
    object->field_81 = 4;
    object->field_04 = object->field_04 + 1;
    object->field_0c = 0;
    object->field_10 = object->pos_x;
    object->field_14 = object->pos_y;
    func_80130700(object, data_801e9f44_slot06_00[0]);
}
