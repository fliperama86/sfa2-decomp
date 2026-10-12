/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4788_slot04_03(Object *obj) {
    int k = 0xc;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    if (obj->hit_flag_a != 0 && obj->ground_y - obj->pos_y >= 0x30 && func_8013ffe4(obj, -0x30, 0x10, 0x20, 0x10) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
        obj->input_dir_0 = 1;
    } else {
        func_80141f28(obj, obj->direction_index >> 1);
        if (obj->variant_index != 0) {
            k = 0x12;
        }
        if (obj->input_dir_second != 0) {
            k += 3;
        }
        func_801307e0(obj, (s16)((obj->direction_index >> 1) + k));
    }
}
