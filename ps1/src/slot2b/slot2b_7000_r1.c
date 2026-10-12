/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079804_slot2b[];
extern ObjectFn data_80079810_slot2b[];
void func_80077040_slot2b(Object *obj);
void func_80077080_slot2b(Object *obj);
void func_80077238_slot2b(Object *obj);
void func_80077444_slot2b(Object *obj);

void func_80077000_slot2b(Object *obj) {
    if (obj->input_dir_0 != 0) {
        func_80077444_slot2b(obj);
    } else {
        func_80077040_slot2b(obj);
    }
}

void func_80077040_slot2b(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        func_80077238_slot2b(obj);
    } else {
        func_80077080_slot2b(obj);
    }
}

void func_80077080_slot2b(Object *obj) {
    data_80079804_slot2b[obj->direction_index >> 1](obj);
}

void func_800770c4_slot2b(Object *obj) {
    data_80079810_slot2b[obj->state_3](obj);
}
