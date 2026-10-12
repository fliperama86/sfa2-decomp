/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b5ef0_slot04_02(Object *obj);

void func_801b5b44_slot04_02(Object *obj) {
    Object *o = obj->other;
    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 3);
    play_sound_at(o, o->side, 0x31a);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if (func_801b5ef0_slot04_02(obj) != 0) {
            obj->facing_side = obj->facing_side + 1;
        }
    } else if (obj->input_bits & 0x8000) {
        obj->facing_side = 1;
    }
    func_801307e0(obj, (obj->input_dir_second >> 1) + 0x18);
}

void func_801b5bf4_slot04_02(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        func_80140770(obj, 0, 5, 0xf, 0, 1, 0);
    }
    update_sequence(obj);
}

void func_801b5c58_slot04_02(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->facing_side = obj->facing_side ^ 1;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}
