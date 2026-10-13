/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"
/* countdown_timer / field_b2 are used as signed 16-bit counters here (hence the s16 views); field_b2 also as a byte at 0xb2. */


void count_down_timer_or_advance_step(GameState *state, Object *object) {
    if (state->field_09 != 0 || state->state_2c_flag != 0) {
        clear_wait_flag(object);
    } else {
        if (--*(s16 *)&object->countdown_timer == 0) {
            object->countdown_timer = 0x7f;
            if (*(s16 *)&object->field_b2 == 0) {
                reset_phase_fields_and_next_step(state, object);
                return;
            }
            (*(s16 *)&object->field_b2)--;
        }
        set_wait_flag(object, 0xe);
    }
}

void reset_phase_fields_and_next_step(GameState *state, Object *object) {
    object->phase_index = 0;
    object->phase_state = 0;
    object->vel_table_index = 0;
    object->countdown_timer = 0;
    object->field_b2 = 0;
    object->cost_index = 0;
    object->field_f1 = 0;
    object->step_index++;
    if (object->side == 0)
        state->side_bits_b &= 0xfe;
    else if (object->side == 1)
        state->side_bits_b &= 0xfd;
    set_wait_flag(object, 0);
}

void update_countdown_timer_on_pad_input(GameState *state, Object *object) {
    if (state->field_09 == 0 && state->state_2c_flag == 0) {
        u16 pad;
        if (object->side != 0)
            pad = pad_2_pressed;
        else
            pad = pad_1_pressed;
        if (pad & 0xfc) {
            if (*(s16 *)&object->field_b2 == 0) {
                object->countdown_timer = 1;
            } else {
                (*(s16 *)&object->field_b2)--;
                object->countdown_timer = 0x60;
            }
        }
    }
}

u8 is_pad_button_pressed_3(GameState *state, Object *object) {
    return is_side_pad_bit_800_pressed(state, object);
}

u8 is_side_pad_bit_800_pressed(GameState *state, Object *object) {
    if (state->state_46_flag == 0 && state->field_09 == 0) {
        u16 pad;
        if (object->side == 0)
            pad = pad_1_pressed;
        else
            pad = pad_2_pressed;
        if (pad & 0x800)
            return 1;
    }
    return 0;
}

void queue_digit_text(GameState *state, Object *object) {
    if (state->state_2c_flag == 0) {
        TextBuf *buf = table_8016f5f4[object->side];
        *buf->buf = *(u8 *)&object->field_b2 + 0x30;
        add_item_to_table(buf);
    }
}

void call_phase_handler_unless_state_2c_flag(GameState *state, Object *object) {
    if (state->state_2c_flag != 0)
        clear_wait_flag(object);
    else
        call_phase_function(state, object);
}

void call_phase_table_unless_state_46_flag(GameState *state, Object *object) {
    if (state->state_46_flag != 0)
        clear_wait_flag(object);
    else
        table_8016f5bc[object->phase_index](state, object);
}

void init_countdown_5a_and_advance_phase(GameState *state, Object *object) {
    object->countdown_timer = 0x5a;
    object->phase_index++;
    set_wait_flag(object, 0xf);
}

void wait_countdown_or_button_then_advance(GameState *state, Object *object) {
    if ((~object->prev_input_bits_2 & object->input_bits_2 & 0xfc) == 0 && --*(s16 *)&object->countdown_timer != 0) {
        set_wait_flag(object, 0xf);
    } else {
        object->countdown_timer = 0x1e;
        object->phase_index++;
    }
}

void count_down_then_clear_scores(GameState *state, Object *object) {
    if (--*(s16 *)&object->countdown_timer == 0) {
        object->score = 0;
        object->max_score = 0;
        object->score_2 = 0;
        object->cost_index = 0;
        object->phase_index++;
        if (object->side == 0)
            state->side_bits &= 0xfe;
        else if (object->side == 1)
            state->side_bits &= 0xfd;
        set_wait_flag(object, 0);
    } else {
        set_wait_flag(object, 0xf);
    }
}

void reset_object_steps_unless_active_flag(GameState *state, Object *object) {
    if (state->active_flag != 0) {
        clear_wait_flag(object);
    } else {
        object->step_index = 0;
        object->phase_index = 0;
        object->phase_state = 0;
        object->vel_table_index = 0;
    }
}

void loop_hud_state_until_fade_done(void) {
    game_state.fade_busy = 0x1f;
    do {
        ((HudWide *)screen_state)->field_48 = 0;
        table_8016f5fc[((HudWide *)screen_state)->state_index >> 1]();
        request_sound_command_and_switch_thread(((HudWide *)screen_state)->fade_arg);
    } while (game_state.fade_busy != 0);
    game_state.wait_flag = 0;
    close_snd_thread_and_switch_main();
}

void update_fade_banks_2_3(void) {
    game_state.fade_done_mask = 0;
    fade_out_palette_bank_2();
    fade_out_palette_bank_3();
    if (game_state.fade_done_mask == 0xc)
        game_state.fade_busy = 0;
}

void update_fade_banks_0_2_3(void) {
    game_state.fade_done_mask = 0;
    fade_out_palette_bank_0();
    fade_out_palette_bank_2();
    fade_out_palette_bank_3();
    if (game_state.fade_done_mask == 0xd)
        game_state.fade_busy = 0;
}

void fade_in_banks_0_2_3_until_done(void) {
    game_state.fade_done_mask = 0;
    fade_in_palette_bank_0();
    fade_in_palette_bank_2();
    fade_in_palette_bank_3();
    if (game_state.fade_done_mask == 0xd)
        game_state.fade_busy = 0;
}

void update_fade_all_banks(void) {
    game_state.fade_done_mask = 0;
    fade_out_palette_bank_0();
    fade_out_palette_bank_1();
    fade_out_palette_bank_2();
    fade_out_palette_bank_3();
    fade_out_palette_bank_4();
    if (game_state.fade_done_mask == 0x1f)
        game_state.fade_busy = 0;
}

void update_fade_bank_0(void) {
    game_state.fade_done_mask = 0;
    fade_out_palette_bank_0();
    if (game_state.fade_done_mask == 1)
        game_state.fade_busy = 0;
}

void fade_out_palette_bank_0(void) {
    if ((u8)fade_palette_bank(0, -1, 0))
        game_state.fade_done_mask |= 1;
    queue_image_upload_job(0, 6);
}

void update_fade_bank_1(void) {
    game_state.fade_done_mask = 0;
    fade_out_palette_bank_1();
    if (game_state.fade_done_mask == 2)
        game_state.fade_busy = 0;
}

void fade_out_palette_bank_1(void) {
    if ((u8)fade_palette_bank(1, -1, 0))
        game_state.fade_done_mask |= 2;
    queue_image_upload_job(1, 0);
}

void update_fade_bank_2(void) {
    game_state.fade_done_mask = 0;
    fade_out_palette_bank_2();
    if (game_state.fade_done_mask == 4)
        game_state.fade_busy = 0;
}

void fade_out_palette_bank_2(void) {
    if ((u8)fade_palette_bank(2, -1, 0))
        game_state.fade_done_mask |= 4;
    queue_image_upload_job(2, 1);
}

void update_fade_bank_3(void) {
    game_state.fade_done_mask = 0;
    fade_out_palette_bank_3();
    if (game_state.fade_done_mask == 8)
        game_state.fade_busy = 0;
}

void fade_out_palette_bank_3(void) {
    if ((u8)fade_palette_bank(3, -1, 0))
        game_state.fade_done_mask |= 8;
    queue_image_upload_job(3, 2);
}

void update_fade_bank_4(void) {
    game_state.fade_done_mask = 0;
    fade_out_palette_bank_4();
    if (game_state.fade_done_mask == 0x10)
        game_state.fade_busy = 0;
}

void fade_out_palette_bank_4(void) {
    if ((u8)fade_palette_bank(4, -1, 0))
        game_state.fade_done_mask |= 0x10;
    queue_image_upload_job(4, 7);
}

void fade_in_palette_banks_0_to_4(void) {
    game_state.fade_done_mask = 0;
    fade_in_palette_bank_0();
    fade_in_palette_bank_1();
    fade_in_palette_bank_2();
    fade_in_palette_bank_3();
    fade_in_palette_bank_4();
    if (game_state.fade_done_mask == 0x1f)
        game_state.fade_busy = 0;
}

void update_fade_bank_0_2(void) {
    game_state.fade_done_mask = 0;
    fade_in_palette_bank_0();
    if (game_state.fade_done_mask == 1)
        game_state.fade_busy = 0;
}

void fade_in_palette_bank_0(void) {
    if ((u8)fade_palette_bank(0, 1, 1))
        game_state.fade_done_mask |= 1;
    queue_image_upload_job(0, 6);
}

void update_fade_bank_1_2(void) {
    game_state.fade_done_mask = 0;
    fade_in_palette_bank_1();
    if (game_state.fade_done_mask == 2)
        game_state.fade_busy = 0;
}

void fade_in_palette_bank_1(void) {
    if ((u8)fade_palette_bank(1, 1, 1))
        game_state.fade_done_mask |= 2;
    queue_image_upload_job(1, 0);
}

void update_fade_bank_2_2(void) {
    game_state.fade_done_mask = 0;
    fade_in_palette_bank_2();
    if (game_state.fade_done_mask == 4)
        game_state.fade_busy = 0;
}

void fade_in_palette_bank_2(void) {
    if ((u8)fade_palette_bank(2, 1, 1))
        game_state.fade_done_mask |= 4;
    queue_image_upload_job(2, 1);
}

void fade_in_bank_3_until_done(void) {
    game_state.fade_done_mask = 0;
    fade_in_palette_bank_3();
    if (game_state.fade_done_mask == 8)
        game_state.fade_busy = 0;
}

void fade_in_palette_bank_3(void) {
    if ((u8)fade_palette_bank(3, 1, 1))
        game_state.fade_done_mask |= 8;
    queue_image_upload_job(3, 2);
}

void fade_in_bank_4_until_done(void) {
    game_state.fade_done_mask = 0;
    fade_in_palette_bank_4();
    if (game_state.fade_done_mask == 0x10)
        game_state.fade_busy = 0;
}

void fade_in_palette_bank_4(void) {
    if ((u8)fade_palette_bank(4, 1, 1))
        game_state.fade_done_mask |= 0x10;
    queue_image_upload_job(4, 7);
}

void update_fade_brighten_all_banks(void) {
    ((HudWide *)screen_state)->countdown--;
    if (((HudWide *)screen_state)->countdown & 0x80) {
        game_state.fade_busy = 0;
    } else {
        fade_palette_bank(0, 1, 2);
        fade_palette_bank(1, 1, 2);
        fade_palette_bank(2, 1, 2);
        fade_palette_bank(3, 1, 2);
        fade_palette_bank(4, 1, 2);
        queue_five_jobs();
    }
}

void update_fade_darken_all_banks(void) {
    ((HudWide *)screen_state)->countdown--;
    if (((HudWide *)screen_state)->countdown & 0x80) {
        game_state.fade_busy = 0;
    } else {
        fade_palette_bank(0, -1, 3);
        fade_palette_bank(1, -1, 3);
        fade_palette_bank(2, -1, 3);
        fade_palette_bank(3, -1, 3);
        fade_palette_bank(4, -1, 3);
        queue_five_jobs();
    }
}

void brighten_banks_0_2_3_with_countdown(void) {
    ((HudWide *)screen_state)->countdown--;
    if (((HudWide *)screen_state)->countdown & 0x80) {
        game_state.fade_busy = 0;
    } else {
        fade_palette_bank(0, 1, 2);
        fade_palette_bank(2, 1, 2);
        fade_palette_bank(3, 1, 2);
        queue_image_upload_job(0, 6);
        queue_image_upload_job(2, 1);
        queue_image_upload_job(3, 2);
    }
}

void darken_banks_0_2_3_with_countdown(void) {
    ((HudWide *)screen_state)->countdown--;
    if (((HudWide *)screen_state)->countdown & 0x80) {
        game_state.fade_busy = 0;
    } else {
        fade_palette_bank(0, -1, 3);
        fade_palette_bank(2, -1, 3);
        fade_palette_bank(3, -1, 3);
        queue_image_upload_job(0, 6);
        queue_image_upload_job(2, 1);
        queue_image_upload_job(3, 2);
    }
}
