/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0690_slot04_01(Object *obj) {
    obj->field_157 = 1;
    if (obj->state_3 == 0) {
        obj->active_flag_3 = 1;
        obj->state_3 = obj->state_3 + 1;
        func_80130dc0(obj);
    } else {
        func_80142a14(obj);
    }
}

void func_801b06dc_slot04_01(Object *obj) {
    int t = 0xc;
    int one = 1;

    obj->state_3 = 3;
    obj->active_flag_3 = one;
    func_80130504(obj);
    func_80141f28(obj, obj->direction_index >> 1);
    if (obj->variant_index != 0) {
        t = 0x12;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    } else if (obj->direction_index != 0) {
        if (obj->pos_y - obj->ground_y < -0x2f && (obj->input_bits_2 & 0xe000) != 0) {
            if (func_8013ffe4(obj, -0x20, 0x20, 0, 0x10) & 0xff) {
                obj->state_1 = 2;
                obj->state_0 = one;
                obj->state_2 = 0;
                obj->state_3 = 0;
                obj->input_dir_0 = 4;
                return;
            }
        }
    }
    func_801307e0(obj, (s16)((obj->direction_index >> 1) + t));
}
