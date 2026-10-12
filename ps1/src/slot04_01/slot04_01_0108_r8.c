/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3b4c_slot04_01(Object *obj);
extern ObjectFnInt data_801beed0_slot04_01[];
extern ObjectFn data_801beefc_slot04_01[];
extern ObjectFn data_801bef28_slot04_01[];
extern ObjectFn data_801bef54_slot04_01[];

void func_801b1070_slot04_01(Object *obj) {
    data_801ad398 = data_801beed0_slot04_01[obj->field_15a](obj);
}

int func_801b10b8_slot04_01(Object *obj) {
    return 1;
}

int func_801b10c0_slot04_01(Object *obj) {
    return obj->linked_object_3 == 0;
}

int func_801b10cc_slot04_01(Object *obj) {
    return obj->active_state != 0;
}

int func_801b10d8_slot04_01(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int func_801b10ec_slot04_01(Object *obj) {
    return obj->remaining_count != 0;
}

void func_801b10f8_slot04_01(Object *obj) {
    data_801beefc_slot04_01[obj->field_15a](obj);
}

void func_801b1138_slot04_01(Object *obj) {
    data_801bef28_slot04_01[obj->field_15a](obj);
}

void func_801b1178_slot04_01(Object *obj) {
    data_801bef54_slot04_01[obj->state_3](obj);
    func_801b3b4c_slot04_01(obj);
}
