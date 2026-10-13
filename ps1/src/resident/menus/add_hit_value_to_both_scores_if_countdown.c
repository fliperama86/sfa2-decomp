/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void add_hit_value_to_both_scores_if_countdown(Entity *entity) {
    u8 s0;
    u16 v;
    if (entity->countdown != 0) {
        s0 = ref_first.p->field_255 + 2;
        v = get_table_value_and_count_hit(ref_first.p);
        ref_first.p->effect_code_bc = s0;
        add_table_bcd_to_player_total(v, ref_first.p->team_id);
        add_table_bcd_score_to_player(v, ref_first.p->team_id);
    }
}

void call_hud_handler_by_state_index(GameState *state) {
    table_8016e7d0[((HudBig *)screen_state)->state_index](state);
    call_input_step_handler();
}

void advance_state_and_set_countdown_timer_2(GameState *state) {
    ((HudBig *)screen_state)->state_index++;
    ((Entity *)state)->field_116 = 0;
    ((Entity *)state)->countdown_timer_2 = 0x32;
    if (((Entity *)state)->countdown != 0) {
        set_other_marker_and_play_side_sound_3(((Entity *)state)->field_7c, (Entity *)state);
    } else if (((Entity *)state)->field_4d == 0 && ((Entity *)state)->nonzero_flag != 0) {
        ((Entity *)state)->countdown_timer_2 = 0x14;
    }
    if (((Entity *)state)->field_78 != 0) {
        ((Entity *)state)->field_88 = ((Entity *)state)->field_78->team_id;
    }
    count_down_then_advance_state_index(state);
}

void count_down_then_advance_state_index(GameState *state) {
    if (((Entity *)state)->countdown != 0) {
        ((Entity *)state)->field_11a = 0xff;
        ((Entity *)state)->countdown--;
    } else {
        ((HudBig *)screen_state)->state_index++;
        ((Entity *)state)->field_6c = 5;
        if (((Entity *)state)->field_11a != 0) {
            set_menu_command_by_code(5, 1);
        }
    }
}

void check_table_value_then_advance_state(GameState *state) {
    u16 v;
    if (((Entity *)state)->table_compare_value != table_8016e7e0[((Entity *)state)->table_index]) {
        update_entity_countdowns_and_sounds((Entity *)state);
    } else {
        ((HudBig *)screen_state)->state_index++;
        if (((Entity *)state)->mode_flag_b != 0) {
            ((Entity *)state)->countdown_timer = 0xa0;
        } else {
            ((Entity *)state)->countdown_timer = 0x5a;
        }
        if (((Entity *)state)->field_4d == 0 && ((Entity *)state)->nonzero_flag == 0) {
            add_round_score_to_other_side(((Entity *)state)->field_7c);
        }
    }
}
