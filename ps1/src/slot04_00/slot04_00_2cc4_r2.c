/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2d98_slot04_00(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->facing_side = 0;
    if (obj->mode_flag_2 == 0) {
        if (obj->input_bits & 0x8000) {
            obj->facing_side = 1;
        }
    }
    func_801307e0(obj, (obj->input_dir_second >> 1) + 0x18);
}

void func_801b2e20_slot04_00(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        func_80140770(obj, 0, 5, 0x11, 0, 1, 0);
    }
    update_sequence(obj);
}
