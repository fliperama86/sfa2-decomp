/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void play_sounds_at_countdown_timer_values_slot12(void);
void spawn_block_with_saved_side_slot12(void);

void wait_then_start_fade_spawn_blocks_advance_slot12(void) {
    Object *o;
    while (game_state.wait_flag) {
        update_blocks();
        update_active_slab172_entries();
        play_sounds_at_countdown_timer_values_slot12();
        request_sound_command_and_switch_thread(1);
    }
    start_fade_mode_2001_no_wait();
    spawn_block_with_saved_side_slot12();
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x7b;
        o->table_index = 0;
        o->spawn_x = 0;
        o->spawn_y = 0x1e1;
    }
    data_8018f5a0->substep_index++;
}

void advance_substep_and_enable_display_slot12(void) {
    data_8018f5a0->substep_index++;
    func_80157d00(1);
}
