/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd4ec_slot05_06[];

void func_801cc61c_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->other->other_state_code = 5;
        func_801312b8(obj);
    }
}

void func_801cc660_slot05_06(Object *obj) {
    data_801dd4ec_slot05_06[obj->state_3](obj);
}

void func_801cc6a0_slot05_06(Object *obj) {
    obj->state_3++;
    func_80141f28(obj, 3);
    play_sound_at(obj->other, obj->other->side, 0x31a);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if ((s16)(box_margin[0] + 0xc0) < obj->pos_x) {
            obj->facing_side = 1;
        }
    } else if ((obj->input_bits & 0x8000) == 0) {
        obj->facing_side = 1;
    }
    func_801307e0(obj, 0x18);
}
