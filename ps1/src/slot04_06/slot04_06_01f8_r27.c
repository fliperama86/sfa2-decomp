/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b571c_slot04_06(Object *obj);
void func_801b583c_slot04_06(Object *obj);
extern ObjectFn data_801c5584_slot04_06[];
extern ObjectFn data_801c5590_slot04_06[];

void func_801b569c_slot04_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b56dc_slot04_06(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        func_801b583c_slot04_06(obj);
    } else {
        func_801b571c_slot04_06(obj);
    }
}

void func_801b571c_slot04_06(Object *obj) {
    data_801c5584_slot04_06[obj->direction_index >> 1](obj);
}

void func_801b5760_slot04_06(Object *obj) {
    data_801c5590_slot04_06[obj->state_3](obj);
}

void func_801b57a0_slot04_06(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    func_80130dc0(obj);
}
