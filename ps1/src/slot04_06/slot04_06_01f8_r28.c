/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5598_slot04_06[];
extern ObjectFn data_801c55a4_slot04_06[];

void func_801b583c_slot04_06(Object *obj) {
    data_801c5598_slot04_06[obj->direction_index >> 1](obj);
}

void func_801b5880_slot04_06(Object *obj) {
    data_801c55a4_slot04_06[obj->state_3](obj);
}

void func_801b58c0_slot04_06(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    func_80130dc0(obj);
}
