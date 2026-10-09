/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5564_slot04_06[];
extern ObjectFn data_801c5570_slot04_06[];

void func_801b5414_slot04_06(Object *obj) {
    data_801c5564_slot04_06[obj->field_12a >> 1](obj);
}

void func_801b5458_slot04_06(Object *obj) {
    data_801c5570_slot04_06[obj->field_07](obj);
}
