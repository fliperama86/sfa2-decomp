/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801dd37c_slot05_06[];
extern ObjectFn data_801dd388_slot05_06[];

void func_801ca048_slot05_06(Object *obj) {
    obj->field_4c = data_801dd37c_slot05_06[obj->field_12a >> 1];
}

void func_801ca070_slot05_06(Object *obj) {
    data_801dd388_slot05_06[obj->field_07](obj);
}
