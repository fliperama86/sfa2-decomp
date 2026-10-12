/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3284_slot04_03(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = 0;
    if ((obj->input_bits & 0x8000) == 0) {
        obj->facing_side = 1;
    }
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    func_80141f28(obj, 3);
    func_801307e0(obj, 0x18);
}

void func_801b32f8_slot04_03(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        game_state.state_param = 0x30;
        if ((func_80140cd8(obj, 0x10, 0) & 0xff) != 0) {
            play_sound_at(obj, obj->side, 0x34b);
        } else {
            play_sound_at(obj, obj->side, 0x319);
        }
        func_80146960(obj);
    }
    update_sequence(obj);
}

void func_801b338c_slot04_03(Object *obj) {
    if (obj->step_flags & 0x80) {
        obj->state_3 = obj->state_3 + 1;
        func_80140770(obj, 0, 0xf, -0x200, 0, 0, 1);
    }
    update_sequence(obj);
}
