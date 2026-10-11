/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b21d0_slot04_0b(Object *o) {
    o->state_3++;
    o->accel_x = 0;
    o->accel_y = 0;
    o->vel_y = 0;
    if (o->field_0b == 0) {
        o->vel_x = 0xfffe0000;
    } else {
        o->vel_x = 0x20000;
    }
    func_801428e4(o);
    func_80138ae8(&game_state, o);
    func_80145d20(o);
    func_801307e0(o, (o->field_12a >> 1) + 0x2d);
}

void func_801b2258_slot04_0b(Object *o) {
    if ((s16)o->step_flags & 0xff00) {
        o->state_3++;
        if (o->field_4b != 0) {
            o->field_165 = 1;
        } else {
            o->field_165 = 0xff;
        }
        func_801483a4(o, -0x28, 0x61);
        play_sound_at(o, o->side, 0x31c);
        play_side_sound_at(o, o->side, 7);
    }
    update_sequence(o);
}
