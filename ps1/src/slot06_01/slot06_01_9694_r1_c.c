/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801eb960_slot06_01[];

void func_801e9a84_slot06_01(Object *obj, u8 index);

void func_801e9a84_slot06_01(Object *obj, u8 index) {
    obj->field_54 = index;
    func_80130700(obj, data_801eb960_slot06_01[index]);
}
