/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd244_slot05_06[];
extern ObjectFn data_801dd24c_slot05_06[];

void func_801c83e8_slot05_06(Object *obj) {
    data_801dd244_slot05_06[obj->state_3](obj);
}

void func_801c8428_slot05_06(Object *obj) {
    obj->state_3++;
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x18, 0x16) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}

void func_801c84b0_slot05_06(Object *obj) {
    func_80142a14(obj);
}

void func_801c84d0_slot05_06(Object *obj) {
    data_801dd24c_slot05_06[obj->state_3](obj);
}
