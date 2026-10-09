/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079834_slot2b[];
extern ObjectFn data_80079840_slot2b[];
void func_80077484_slot2b(Object *obj);
void func_800775a4_slot2b(Object *obj);

void func_800773a0_slot2b(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_80077404_slot2b(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_45 = 0;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_80077444_slot2b(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_800775a4_slot2b(obj);
    } else {
        func_80077484_slot2b(obj);
    }
}

void func_80077484_slot2b(Object *obj) {
    data_80079834_slot2b[obj->field_12a >> 1](obj);
}

void func_800774c8_slot2b(Object *obj) {
    data_80079840_slot2b[obj->field_07](obj);
}
