/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80027c6c_slot27[];
extern ObjectFn data_80027c7c_slot27[];
extern ObjectFn data_80027cac_slot27[];
void func_80014c1c_slot27(Object *obj);
void func_80014cd8_slot27(Object *obj);

void func_800143ac_slot27(Object *obj) {
    data_80027c6c_slot27[obj->state_0](obj);
}

void func_800143ec_slot27(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
    func_80014c1c_slot27(obj);
    func_80014cd8_slot27(obj);
}

void func_8001442c_slot27(Object *obj) {
    data_80027c7c_slot27[obj->table_index](obj);
    advance_sequence_checked(obj);
}

void func_80014480_slot27(Object *obj) {
    data_80027cac_slot27[obj->state_1](obj);
}

void func_800144c0_slot27(Object *obj) {
    obj->spawn_x = 0x20;
    obj->spawn_y = 0x1e0;
    obj->field_01 = 1;
    obj->state_1 = obj->state_1 + 1;
    if (obj->table_index == 10) {
        obj->vel_x = 8;
    } else {
        obj->vel_x = -8;
    }
}
