/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void spawn_pooled_object_with_index_by_player_flags_slot01(void) {
    Object *p = (Object *)pop_block_from_pool();
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0x30;
        p->table_index = 0x80;
        if (game_state.mode != 3) {
            if ((player_left.field_f1 | player_right.field_f1) != 0) {
                p->table_index = 0x82;
            }
        }
    }
}

void start_wait_with_two_halfword_stores_slot01(void) {
    if (game_state.fade_busy == 0) {
        game_state.wait_flag = 0x1f;
        ((u16 *)data_80190464)[0] = 0x2201;
        ((u16 *)data_8019046c)[0] = 0x1e00;
        set_voice_thread_entry_and_open(2, 4);
    }
}

void loop_updates_while_wait_flag_slot01(void) {
    while (game_state.wait_flag != 0) {
        update_blocks();
        call_active_block_handlers();
        select_buffers_and_run_first_setup();
        request_sound_command_and_switch_thread(1);
    }
}
