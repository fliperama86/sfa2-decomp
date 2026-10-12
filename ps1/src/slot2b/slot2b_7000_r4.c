/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079834_slot2b[];
extern ObjectFn data_80079840_slot2b[];
void func_80077484_slot2b(Object *obj);
void func_800775a4_slot2b(Object *obj);

void func_800773a0_slot2b(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_80077404_slot2b(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->active_state = 0;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_80077444_slot2b(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        func_800775a4_slot2b(obj);
    } else {
        func_80077484_slot2b(obj);
    }
}

void func_80077484_slot2b(Object *obj) {
    data_80079834_slot2b[obj->direction_index >> 1](obj);
}

void func_800774c8_slot2b(Object *obj) {
    data_80079840_slot2b[obj->state_3](obj);
}
