/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b06e4_slot04_02(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_3 = 1;
    func_80130dc0(obj);
}

void func_801b0714_slot04_02(Object *obj) {
    u16 x;

    obj->input_dir_0 = 4;
    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->direction_index >> 1);
    x = 0xc;
    if (obj->variant_index != 0) {
        x = 0x12;
    }
    if (obj->input_dir_second != 0) {
        x += 3;
    }
    x += obj->direction_index >> 1;
    func_801307e0(obj, x);
    if ((u16)obj->pos_y < (u16)(obj->ground_y - 0x50) && obj->variant_index != 0 && (obj->variant_index & 0x80) == 0 && obj->input_dir_second != 0 && obj->direction_index == 2 && (obj->input_bits_2 & 0x4000) != 0) {
        func_801b631c_slot04_02(obj, 1, 0, 5, 0);
        func_801307e0(obj, 0x29);
    }
}
