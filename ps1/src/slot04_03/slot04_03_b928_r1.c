/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0928_slot04_03(Object *obj) {
    int t = 0xf;
    int one = 1;

    obj->state_3 = 3;
    obj->active_flag_3 = one;
    func_80130504(obj);
    func_80141f28(obj, obj->direction_index >> 1);
    if (obj->variant_index != 0) {
        t = 0x15;
    }
    if (obj->input_dir_second == 0) {
        t -= 3;
        if (obj->direction_index != 0) {
            if (obj->ground_y - obj->pos_y >= 0x30 && (obj->input_bits_2 & 0xe000) != 0) {
                if (func_8013ffe4(obj, -0x30, 0x10, 8, 0x10)) {
                    obj->state_0 = one;
                    obj->state_1 = 2;
                    obj->state_2 = 0;
                    obj->state_3 = 0;
                    obj->input_dir_0 = one;
                    return;
                }
            }
        }
    }
    func_801307e0(obj, (s16)((obj->direction_index >> 1) + t));
}

