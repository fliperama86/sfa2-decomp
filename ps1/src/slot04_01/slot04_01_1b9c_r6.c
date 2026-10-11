/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b23c8_slot04_01(Object *o) {
    *(s32 *)&o->vel_x = 0x80000;
    o->accel_x = -0x8000;
    o->state_3++;
    func_80145d20(o);
    func_801428e4(o);
    func_80138b38(&game_state, o);
    play_side_sound_at(o, o->side, 0xd);
    func_801307e0(o, (o->field_12a >> 1) + 0x28);
}

void func_801b2448_slot04_01(Object *o) {
    if ((u8)o->step_flags != 0) {
        o->state_3++;
        if (o->field_4b == 0) {
            o->field_165 = 0xff;
        } else {
            o->field_165 = 1;
        }
        play_sound_at(o, o->side, 0x31c);
        func_801483a4(o, -0x18, 0x1c);
    }
    update_sequence(o);
}
