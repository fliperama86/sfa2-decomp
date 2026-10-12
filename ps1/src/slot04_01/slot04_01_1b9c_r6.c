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
    set_flag_225_add_direction_to_counter(&game_state, o);
    play_side_sound_at(o, o->side, 0xd);
    set_sequence_by_side_and_mode(o, (o->direction_index >> 1) + 0x28);
}

void update_sequence_spawn_on_flag_low_byte_slot04_01(Object *o) {
    if ((u8)o->step_flags != 0) {
        o->state_3++;
        if (o->field_4b == 0) {
            o->field_165 = 0xff;
        } else {
            o->field_165 = 1;
        }
        play_sound_at(o, o->side, 0x31c);
        spawn_children_per_direction_index(o, -0x18, 0x1c);
    }
    update_sequence(o);
}
