/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1068_slot04_07(Object *obj) {
    int a;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    obj->input_dir_0 = 4;
    func_80130504(obj);
    a = 0xc;
    if (obj->input_dir_second == 0) {
        if (obj->direction_index == 2 && (obj->input_bits_2 & 0x4000)) {
            obj->vel_x >>= 1;
            play_side_sound_at(obj, obj->side, 7);
            func_801307e0(obj, 0x1f);
            return;
        }
        if (obj->direction_index != 0 && (obj->input_bits_2 & 0xe000) && obj->pos_y < obj->ground_y - 0x30) {
            if ((u8)func_8013ffe4(obj, -0x20, 0x20, 0, 0x20)) {
                obj->state_0 = 1;
                obj->state_1 = 2;
                obj->state_2 = 0;
                obj->state_3 = 0;
                return;
            }
        }
    }
    if (obj->variant_index != 0) {
        a = 0x12;
    }
    if (obj->input_dir_second != 0) {
        a += 3;
    }
    a += obj->direction_index >> 1;
    func_801307e0(obj, (s16)a);
    func_80141f28(obj, obj->direction_index >> 1);
}
