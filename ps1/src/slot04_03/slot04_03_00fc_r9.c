/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c05f8_slot04_03[];
extern ObjectFn data_801c0604_slot04_03[];
extern ObjectFn data_801c061c_slot04_03[];
extern ObjectFn data_801c0638_slot04_03[];

void call_handler_by_direction_index_slot04_03(Object *obj) {
    data_801c05f8_slot04_03[obj->direction_index >> 1](obj);
}

void func_801b1878_slot04_03(Object *obj) {
    data_801c0604_slot04_03[obj->state_3](obj);
}

void func_801b18b8_slot04_03(Object *obj) {
    data_801c061c_slot04_03[obj->state_3](obj);
}

void func_801b18f8_slot04_03(Object *obj) {
    data_801c0638_slot04_03[obj->state_3](obj);
}
