/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"
/* countdown_timer / field_b2 are used as signed 16-bit counters here (hence the s16 views); field_b2 also as a byte at 0xb2. */


void func_80126cd0(GameState *state, Object *object) {
    if (state->field_09 != 0 || state->state_2c_flag != 0) {
        func_801269e0(object);
    } else {
        if (--*(s16 *)&object->countdown_timer == 0) {
            object->countdown_timer = 0x7f;
            if (*(s16 *)&object->field_b2 == 0) {
                func_80126d68(state, object);
                return;
            }
            (*(s16 *)&object->field_b2)--;
        }
        func_80135c80(object, 0xe);
    }
}

void func_80126d68(GameState *state, Object *object) {
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
    func_80135c80(object, 0);
}

void func_80126de8(GameState *state, Object *object) {
    if (state->field_09 == 0 && state->state_2c_flag == 0) {
        u16 pad;
        if (object->side != 0)
            pad = data_801a6976;
        else
            pad = data_801a696a;
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

u8 func_80126e74(GameState *state, Object *object) {
    return func_80126e98(state, object);
}

u8 func_80126e98(GameState *state, Object *object) {
    if (state->state_46_flag == 0 && state->field_09 == 0) {
        u16 pad;
        if (object->side == 0)
            pad = data_801a696a;
        else
            pad = data_801a6976;
        if (pad & 0x800)
            return 1;
    }
    return 0;
}

void func_80126efc(GameState *state, Object *object) {
    if (state->state_2c_flag == 0) {
        TextBuf *buf = table_8016f5f4[object->side];
        *buf->buf = *(u8 *)&object->field_b2 + 0x30;
        func_801519b4(buf);
    }
}

void func_80126f54(GameState *state, Object *object) {
    if (state->state_2c_flag != 0)
        func_801269e0(object);
    else
        func_801564b0(state, object);
}

void func_80126f94(GameState *state, Object *object) {
    if (state->state_46_flag != 0)
        func_801269e0(object);
    else
        table_8016f5bc[object->phase_index](state, object);
}

void func_80126ff4(GameState *state, Object *object) {
    object->countdown_timer = 0x5a;
    object->phase_index++;
    func_80135c80(object, 0xf);
}

void func_8012702c(GameState *state, Object *object) {
    if ((~object->prev_input_bits_2 & object->input_bits_2 & 0xfc) == 0 && --*(s16 *)&object->countdown_timer != 0) {
        func_80135c80(object, 0xf);
    } else {
        object->countdown_timer = 0x1e;
        object->phase_index++;
    }
}

void func_801270a4(GameState *state, Object *object) {
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
        func_80135c80(object, 0);
    } else {
        func_80135c80(object, 0xf);
    }
}

void func_80127140(GameState *state, Object *object) {
    if (state->active_flag != 0) {
        func_801269e0(object);
    } else {
        object->step_index = 0;
        object->phase_index = 0;
        object->phase_state = 0;
        object->vel_table_index = 0;
    }
}

void func_80127188(void) {
    game_state.fade_busy = 0x1f;
    do {
        ((HudWide *)data_8018f5a0)->field_48 = 0;
        table_8016f5fc[((HudWide *)data_8018f5a0)->state_index >> 1]();
        func_801192bc(((HudWide *)data_8018f5a0)->fade_arg);
    } while (game_state.fade_busy != 0);
    game_state.wait_flag = 0;
    func_801192f0();
}

void func_80127228(void) {
    game_state.fade_done_mask = 0;
    func_80127510();
    func_801275ac();
    if (game_state.fade_done_mask == 0xc)
        game_state.fade_busy = 0;
}

void func_80127278(void) {
    game_state.fade_done_mask = 0;
    func_801273d8();
    func_80127510();
    func_801275ac();
    if (game_state.fade_done_mask == 0xd)
        game_state.fade_busy = 0;
}

void func_801272d0(void) {
    game_state.fade_done_mask = 0;
    func_8012774c();
    func_80127884();
    fade_in_palette_bank_3();
    if (game_state.fade_done_mask == 0xd)
        game_state.fade_busy = 0;
}

void func_80127328(void) {
    game_state.fade_done_mask = 0;
    func_801273d8();
    func_80127474();
    func_80127510();
    func_801275ac();
    func_80127648();
    if (game_state.fade_done_mask == 0x1f)
        game_state.fade_busy = 0;
}

void func_80127390(void) {
    game_state.fade_done_mask = 0;
    func_801273d8();
    if (game_state.fade_done_mask == 1)
        game_state.fade_busy = 0;
}

void func_801273d8(void) {
    if ((u8)fade_palette_bank(0, -1, 0))
        game_state.fade_done_mask |= 1;
    func_80137220(0, 6);
}

void func_8012742c(void) {
    game_state.fade_done_mask = 0;
    func_80127474();
    if (game_state.fade_done_mask == 2)
        game_state.fade_busy = 0;
}

void func_80127474(void) {
    if ((u8)fade_palette_bank(1, -1, 0))
        game_state.fade_done_mask |= 2;
    func_80137220(1, 0);
}

void func_801274c8(void) {
    game_state.fade_done_mask = 0;
    func_80127510();
    if (game_state.fade_done_mask == 4)
        game_state.fade_busy = 0;
}

void func_80127510(void) {
    if ((u8)fade_palette_bank(2, -1, 0))
        game_state.fade_done_mask |= 4;
    func_80137220(2, 1);
}

void func_80127564(void) {
    game_state.fade_done_mask = 0;
    func_801275ac();
    if (game_state.fade_done_mask == 8)
        game_state.fade_busy = 0;
}

void func_801275ac(void) {
    if ((u8)fade_palette_bank(3, -1, 0))
        game_state.fade_done_mask |= 8;
    func_80137220(3, 2);
}

void func_80127600(void) {
    game_state.fade_done_mask = 0;
    func_80127648();
    if (game_state.fade_done_mask == 0x10)
        game_state.fade_busy = 0;
}

void func_80127648(void) {
    if ((u8)fade_palette_bank(4, -1, 0))
        game_state.fade_done_mask |= 0x10;
    func_80137220(4, 7);
}

void func_8012769c(void) {
    game_state.fade_done_mask = 0;
    func_8012774c();
    func_801277e8();
    func_80127884();
    fade_in_palette_bank_3();
    fade_in_palette_bank_4();
    if (game_state.fade_done_mask == 0x1f)
        game_state.fade_busy = 0;
}

void func_80127704(void) {
    game_state.fade_done_mask = 0;
    func_8012774c();
    if (game_state.fade_done_mask == 1)
        game_state.fade_busy = 0;
}

void func_8012774c(void) {
    if ((u8)fade_palette_bank(0, 1, 1))
        game_state.fade_done_mask |= 1;
    func_80137220(0, 6);
}

void func_801277a0(void) {
    game_state.fade_done_mask = 0;
    func_801277e8();
    if (game_state.fade_done_mask == 2)
        game_state.fade_busy = 0;
}

void func_801277e8(void) {
    if ((u8)fade_palette_bank(1, 1, 1))
        game_state.fade_done_mask |= 2;
    func_80137220(1, 0);
}

void func_8012783c(void) {
    game_state.fade_done_mask = 0;
    func_80127884();
    if (game_state.fade_done_mask == 4)
        game_state.fade_busy = 0;
}

void func_80127884(void) {
    if ((u8)fade_palette_bank(2, 1, 1))
        game_state.fade_done_mask |= 4;
    func_80137220(2, 1);
}

void func_801278d8(void) {
    game_state.fade_done_mask = 0;
    fade_in_palette_bank_3();
    if (game_state.fade_done_mask == 8)
        game_state.fade_busy = 0;
}

void fade_in_palette_bank_3(void) {
    if ((u8)fade_palette_bank(3, 1, 1))
        game_state.fade_done_mask |= 8;
    func_80137220(3, 2);
}

void func_80127974(void) {
    game_state.fade_done_mask = 0;
    fade_in_palette_bank_4();
    if (game_state.fade_done_mask == 0x10)
        game_state.fade_busy = 0;
}

void fade_in_palette_bank_4(void) {
    if ((u8)fade_palette_bank(4, 1, 1))
        game_state.fade_done_mask |= 0x10;
    func_80137220(4, 7);
}

void func_80127a10(void) {
    ((HudWide *)data_8018f5a0)->countdown--;
    if (((HudWide *)data_8018f5a0)->countdown & 0x80) {
        game_state.fade_busy = 0;
    } else {
        fade_palette_bank(0, 1, 2);
        fade_palette_bank(1, 1, 2);
        fade_palette_bank(2, 1, 2);
        fade_palette_bank(3, 1, 2);
        fade_palette_bank(4, 1, 2);
        func_80137b10();
    }
}

void func_80127ac4(void) {
    ((HudWide *)data_8018f5a0)->countdown--;
    if (((HudWide *)data_8018f5a0)->countdown & 0x80) {
        game_state.fade_busy = 0;
    } else {
        fade_palette_bank(0, -1, 3);
        fade_palette_bank(1, -1, 3);
        fade_palette_bank(2, -1, 3);
        fade_palette_bank(3, -1, 3);
        fade_palette_bank(4, -1, 3);
        func_80137b10();
    }
}

void func_80127b78(void) {
    ((HudWide *)data_8018f5a0)->countdown--;
    if (((HudWide *)data_8018f5a0)->countdown & 0x80) {
        game_state.fade_busy = 0;
    } else {
        fade_palette_bank(0, 1, 2);
        fade_palette_bank(2, 1, 2);
        fade_palette_bank(3, 1, 2);
        func_80137220(0, 6);
        func_80137220(2, 1);
        func_80137220(3, 2);
    }
}

void func_80127c28(void) {
    ((HudWide *)data_8018f5a0)->countdown--;
    if (((HudWide *)data_8018f5a0)->countdown & 0x80) {
        game_state.fade_busy = 0;
    } else {
        fade_palette_bank(0, -1, 3);
        fade_palette_bank(2, -1, 3);
        fade_palette_bank(3, -1, 3);
        func_80137220(0, 6);
        func_80137220(2, 1);
        func_80137220(3, 2);
    }
}
