/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6a84_slot04_02(Object *obj);
void func_801b68b4_slot04_02(Object *obj);
void func_801b6cb0_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b6a40_slot04_02(Object *obj) {
    if (obj->limit_lo & 1) {
        func_801b68b4_slot04_02(obj);
    } else {
        func_801b6a84_slot04_02(obj);
    }
}

void func_801b6a84_slot04_02(Object *obj) {
    int t;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    func_80120af8(obj);
    t = obj->direction_index >> 1;
    func_80141f28(obj, t);
    if (obj->variant_index != 0) {
        t += 0x12;
    } else {
        t += 0xc;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    }
    func_801307e0(obj, (s16)t);
    func_80120af8(obj);
    if (obj->pos_y < (u16)(((Slot04bObj *)obj)->ground_y - 0x50) && obj->variant_index != 0 && (obj->variant_index & 0x80) == 0 && obj->input_dir_second != 0 && obj->direction_index == 2 && obj->attack_param_d != 0) {
        func_801b6cb0_slot04_02(obj, 1, 0, 5, 0);
        func_801307e0(obj, 0x29);
    }
}
