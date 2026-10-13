/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void update_cells_from_entry_table_slot28(Object *o);

void count_down_then_fade_out_step_slot28(int *p) {
    int t = screen_state->countdown_timer - 1;
    screen_state->countdown_timer = t;
    if ((s16)t < 0) {
        screen_state->step_index_2++;
        start_fade_mode_1_and_wait();
    }
    update_cells_from_entry_table_slot28((Object *)p);
    update_blocks();
    update_active_slab172_entries();
    set_menu_command_by_code(6, 2);
}

void finish_step_when_wait_flag_clear_slot28(int *p) {
    update_cells_from_entry_table_slot28((Object *)p);
    update_blocks();
    update_active_slab172_entries();
    if (game_state.wait_flag == 0) {
        kind_handlers_enabled = 0;
        screen_state->step_index_2 = 0;
        screen_state->pass_index = 0;
        screen_state->substep_index++;
    }
}

void init_sixteen_records_to_eight_slot28(int *p) {
    int i;
    for (i = 0; i < 16; i++) {
        Slot28Rec51cf0 *c = (Slot28Rec51cf0 *)(p + 3) + i;
        c->field_00 = 0;
        c->init_value_b = 8;
        c->init_value_a = 8;
        c->field_0a = 0;
    }
}
