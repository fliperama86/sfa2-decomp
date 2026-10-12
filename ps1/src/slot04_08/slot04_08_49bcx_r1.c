#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b49bc_slot04_08(Object *obj) {
    int s = 0xc;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    func_80141f28(obj, obj->direction_index >> 1);
    if (obj->variant_index != 0) {
        s = 0x12;
    }
    if (obj->input_dir_second != 0) {
        s += 3;
    }
    if (obj->attack_param_d != 0) {
        play_side_sound_at(obj, obj->side, 0xc);
        func_80141f28(obj, 2);
        func_801307e0(obj, 0x34);
    } else {
        func_801307e0(obj, (s16)((obj->direction_index >> 1) + s));
    }
}
