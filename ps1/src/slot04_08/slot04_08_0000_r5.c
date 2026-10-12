/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0658_slot04_08(Object *obj) {
    int k;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->direction_index >> 1);
    k = 0xc;
    if (obj->variant_index != 0) {
        k = 0x12;
    }
    if (obj->input_dir_second != 0) {
        k += 3;
    } else if (obj->direction_index == 4 && (obj->input_bits_2 & 0x4000) != 0) {
        play_side_sound_at(obj, obj->side, 0xc);
        func_80141f28(obj, 2);
        func_801307e0(obj, 0x34);
        return;
    }
    func_801307e0(obj, (s16)((obj->direction_index >> 1) + k));
}
