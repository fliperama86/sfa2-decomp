/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0b74_slot04_0a(Object *obj) {
    int a;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->direction_index >> 1);
    a = 0x15;
    if (obj->variant_index == 0) {
        a = 0xf;
    }
    if (obj->input_dir_second == 0) {
        a -= 3;
        if (obj->direction_index != 0 && obj->pos_y < obj->ground_y - 0x30 && (obj->input_bits_2 & 0xe000) != 0 && (u8)func_8013ffe4(obj, -0x20, 0x20, 0, 0x10) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            obj->input_dir_0 = 4;
            return;
        }
    }
    func_801307e0(obj, (s16)((obj->direction_index >> 1) + a));
}
