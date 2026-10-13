/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void link_players(GameState *state, Object *a1, Object *a2) {
    a1->other = &player_left + 1;
    a2->other = &player_left;
    a1->image_row = 0x10;
    a2->image_row = 0xb;
    state->player_left_ptr = &player_left;
    state->player_right_ptr = &player_left + 1;
    if (a1->mode_flag_2 != 0) {
        a1->field_d4 = 0;
        if (a1->kind == a2->kind && a2->field_d4 == 0) {
            a1->field_d4 = 1;
        }
    }
    if (a2->mode_flag_2 != 0) {
        a2->field_d4 = 0;
        if (a1->kind == a2->kind && a1->field_d4 == 0) {
            a2->field_d4 = 1;
        }
    }
}

void call_handler_by_substep_index(GameState *state) {
    if (screen_state->substep_index == 0) {
        advance_substep_and_reset_object_ids((Object *)state);
    } else if (screen_state->substep_index == 1) {
        call_step_handler_then_update_blocks_and_buffers((Object *)state);
    }
}

void advance_substep_and_reset_object_ids(Object *object) {
    screen_state->substep_index++;
    object->phase_state = 0;
    spawn_position_table_objects_and_list_objects();
    func_801e1d0c(0);
    object->state_2 = 0xff;
    object->team_id = 0xff;
}

void call_step_handler_then_update_blocks_and_buffers(Object *object) {
    u8 *flag = &single_object_active;
    table_8016e774[screen_state->step_index_2](object);
    if (*flag != 0) {
        func_801e1b64();
        func_801e1d6c(flag);
    }
    update_blocks();
    call_active_block_handlers();
    set_capped_value_and_ot_indices(object);
    select_buffers_and_run_first_setup();
}

void advance_step_and_finish_on_field_af(Object *object) {
    screen_state->step_index_2++;
    object->field_dc |= 0xe;
    if (object->countdown_2 != 0) {
        screen_state->step_index_2 = 0xc;
        screen_state->countdown_timer = 0x10;
        object->ot_depth = 1;
        object->linked_object_2 = 0xff;
        init_hud_primitives();
        object->phase_state = 0;
        player_left.field_01 = 0;
        player_right.field_01 = 0;
    }
}
