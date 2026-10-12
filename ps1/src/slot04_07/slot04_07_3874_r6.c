/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);

void func_801b3ecc_slot04_07(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 4);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    play_side_sound_at(obj, obj->side, 9);
    *(s32 *)&obj->vel_x = 0;
    *(s32 *)&obj->accel_x = 0;
    obj->facing_side = 0;
    if ((obj->input_bits & 0x8000) != 0) {
        obj->facing_side = 1;
    }
    obj->countdown = 0xc00;
    func_801307e0(obj, 0x1b);
}

void func_801b3f5c_slot04_07(Object *obj) {
    int t = obj->countdown - 0x100;
    obj->countdown = t;
    if ((s16)t == 0) {
        obj->state_3++;
    }
}

void func_801b3f90_slot04_07(Object *obj) {
    if (func_801b41c4_slot04_07(obj) >= 0 || obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
        game_state.state_param = 0x40;
        play_side_sound_at(obj, obj->side, 8);
        func_80140cd8(obj, 0xe, 0);
        play_sound_at(obj, obj->side, 0x319);
        func_801307e0(obj, 0x1c);
    }
}
