/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3cc4_slot04_08[];

void start_next_sequence_from_step_table_slot04_08(Object *obj);

void count_down_then_call_next_table_step_slot04_08(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        start_next_sequence_from_step_table_slot04_08(obj);
    }
}

void flip_facing_and_start_sequence_by_direction_slot04_08(Object *obj) {
    int a = 0x64;

    obj->state_3++;
    obj->facing_side ^= 1;
    if (obj->direction_index != 0) {
        a = 0x78;
    }
    set_sequence_by_side_and_mode(obj, a);
}

void deal_step_indexed_damage_spawn_on_defeat_slot04_08(Object *obj) {

    u16 t = obj->step_flags;

    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        game_state.state_param = 0x18;
        spawn_two_effects_at_other_ground(obj);
        if ((u8)apply_damage_to_other_return_defeated(obj, (s16)data_801c3cc4_slot04_08[((Slot04aObj *)obj)->step_index + (obj->direction_index << 1)], 0) != 0) {
            game_state.spawn_kind = 4;
            spawn_game_state_kind_object(obj);
        }
        obj->countdown = 0x10;
        obj->state_3++;
        play_sound_at(obj, ((Slot04aObj *)obj)->side, 0x319);
    }
    update_sequence(obj);
}
