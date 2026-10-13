/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void loop_init_state_and_load_game_state_from_table(void) {
    int z = 0;
    GameState *g = &game_state;
    Tune2a *t = &table_8016e664;
    HudState *h;
    func_80157d00(1);
    screen_state->state_index = 0;
    for (;;) {
        h = screen_state;
        switch (h->state_index) {
        case 0:
            data_8019093c = z;
            data_80190940 = z;
            clear_players_arrays_state_and_init_pools();
            init_sprite_tables();
            load_images_to_vram();
            if (*(int *)0x1f800000 == 0) {
                start_sound_and_set_volumes();
            }
            screen_state->state_index = screen_state->state_index + 1;
            break;
        case 1:
            h->state_index = h->state_index + 1;
        case 2:
            g->saved_value = 1;
            g->field_0f = z;
            g->field_2b = z;
            g->field_ba = z;
            g->table_value_a = t->field_21;
            g->setup_param = t->field_22;
            if (t->override_flag == 0) {
                g->table_value_b = t->field_23;
            } else {
                g->table_value_b = 0xff;
            }
            g->table_value_c = t->field_25 + 1;
            g->handler_index = t->field_26;
            g->table_index = t->field_28;
            g->field_2a = t->field_29;
            screen_state->state_index = 0;
            screen_state->phase_index = 0;
            screen_state->step_index = 0;
            screen_state->substep_index = 0;
            screen_state->step_index_2 = 0;
            switch_to_next_table_entry_and_wait();
            break;
        }
        request_sound_command_and_switch_thread(1);
    }
}
