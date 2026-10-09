/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c3cdc_slot04_08[];
extern ObjectFn data_801c3ce8_slot04_08[];
extern ObjectFn data_801c3d04_slot04_08[];
extern ObjectFn data_801c3d20_slot04_08[];

void func_801b2af4_slot04_08(Object *obj) {
    data_801c3cdc_slot04_08[obj->field_12a >> 1](obj);
}

void func_801b2b38_slot04_08(Object *obj) {
    data_801c3ce8_slot04_08[obj->field_07](obj);
}

void func_801b2b78_slot04_08(Object *obj) {
    data_801c3d04_slot04_08[obj->field_07](obj);
}

void func_801b2bb8_slot04_08(Object *obj) {
    data_801c3d20_slot04_08[obj->field_07](obj);
}
