/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void init_match_state_and_players(GameState *g) {
    Object *l;
    Object *r;
    int z;
    z = 0;
    l = &player_left;
    data_8018f5a0->step_index++;
    data_8018f5a0->substep_index = 0;
    g->timer_or_bits = 0;
    data_8018f5a0->step_index_2 = 0;
    g->countdown_timer_2 = 0;
    data_8018f5a0->pass_index = 0;
    g->field_cc = 0;
    g->input_enable_bits = 0;
    g->pause_flag_2 = 0;
    g->count_index = 0;
    g->field_4d = 0;
    g->side_bits_2 = 0;
    g->active_flag = 0;
    g->field_4f = 0;
    g->gate_flag = 0;
    g->field_6c = 0;
    g->field_60 = 0;
    g->field_6d = 0;
    g->field_64 = 0;
    g->countdown_timer = 0;
    g->countdown_timer_b = 0;
    g->reload_value = 0;
    g->field_87 = 0;
    g->field_83 = 0;
    g->status_flags = 0;
    g->field_65 = 0;
    g->spawn_flag = 0;
    g->select_flag = 0;
    g->field_10c = 0;
    g->state_param = 0;
    g->field_af = 0;
    g->field_84 = 0;
    g->field_09 = 1;
    g->stage_index = 0;
    g->field_4e = 0xff;
    g->state_2c_flag = 0xff;
    g->field_a9 = 0;
    g->reset_value = 7;
    g->mode_copy = g->mode;
    g->mode_copy_2 = g->saved_value;
    g->field_48 = g->table_value_c;
    player_left.field_165 = 0;
    player_left.win_count = 0;
    player_left.meter = 0;
    player_left.effect_code_bc = 0;
    player_left.effect_request = 0;
    player_left.sound_index = 0;
    player_left.field_bf = 0;
    player_left.bytes_d0[0] = 0;
    player_left.bytes_d0[1] = 0;
    player_left.bytes_d0[2] = 0;
    player_left.bytes_d0[3] = 0;
    player_left.max_value_cc = 0;
    player_right.field_165 = 0;
    player_right.win_count = 0;
    player_right.meter = 0;
    player_right.effect_code_bc = 0;
    player_right.effect_request = 0;
    player_right.sound_index = 0;
    player_right.field_bf = 0;
    player_right.bytes_d0[0] = 0;
    player_right.bytes_d0[1] = 0;
    player_right.bytes_d0[2] = 0;
    player_right.bytes_d0[3] = 0;
    player_right.max_value_cc = 0;
    r = l + 1;
    if (g->mode == 3) {
        g->select_flag = 0xff;
        g->field_77 = 0;
        player_left.score = 0;
        player_right.score = 0;
    }
    g->init_timer_value = 0x3c;
    g->round_time = g->table_value_b;
    if (g->mode3_active_flag == 0 && g->versus_flag == 0) {
        check_repeat_count_trigger_kind_code(g);
        check_hit_count_trigger_kind_swap(g);
    }
    set_random_flag_by_chance_when_table_index_6(g);
    link_players(g, l, r);
    g->state_2c_flag = 1;
    clear_arrays_and_init_pools_and_sprite_tables();
    update_table_columns_unless_mode3_inactive(g);
    load_kind_images_to_vram(game_state.kind_index);
    load_player_sound_banks(data_801a8067, data_801a83fb);
    store_all_slot_image_rects_and_queue_jobs();
    set_menu_command_one_and_wait_pending(0x604);
    request_sound_command_and_switch_thread(1);
    func_80157d00(1);
    data_80190568 = 1;
    data_801a6938 = z;
    close_voice_thread(2);
}
