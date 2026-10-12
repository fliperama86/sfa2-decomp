/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b5ef0_slot04_02(Object *obj);
void func_801b5bf4_slot04_02(Object *obj);
u8 func_801b5eb8_slot04_02(Object *obj);

void func_801b5ce8_slot04_02(Object *obj) {
    Object *o = obj->other;
    int t;
    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 3);
    play_sound_at(o, o->side, 0x31a);
    obj->facing_side = 0;
    obj->vel_x = 0x48000;
    if (obj->mode_flag_2 != 0) {
        t = func_801b5ef0_slot04_02(obj);
    } else {
        t = obj->input_bits & 0x8000;
    }
    if (t) {
        obj->vel_x = -0x48000;
        obj->facing_side = obj->facing_side + 1;
    }
    obj->vel_y = 0x20000;
    obj->accel_y = -0x2000;
    func_801307e0(obj, 0x45);
}

void func_801b5da8_slot04_02(Object *o) {
    if (*(u8 *)&o->step_flags == 0) {
        o->active_state = 1;
        o->state_3++;
    }
    update_sequence(o);
}

void func_801b5de8_slot04_02(Object *obj) {
    if (func_801b5eb8_slot04_02(obj)) {
        if (obj->ground_y <= obj->pos_y) {
            obj->pos_y = obj->ground_y;
            game_state.state_param = 0x18;
        }
    }
    func_801b5bf4_slot04_02(obj);
}
