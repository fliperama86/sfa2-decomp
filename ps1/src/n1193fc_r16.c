/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_800100a4(void);
void func_80010e60(void);

void update_phase_or_init_object_pools(GameState *state) {
    HudState *h;
    if (data_8016e68b == 0) {
        h = data_8018f5a0;
        if (h->pass_index == 0) {
            h->pass_index++;
            load_command_5_for_kind_and_wait(game_state.focus_object->kind);
        } else {
            func_800100a4();
        }
    } else if (((Entity *)state)->ready_flag == 0) {
        h = data_8018f5a0;
        h->step_index++;
        ((Entity *)state)->field_65 = 0;
    } else {
        data_8018f5a0->phase_index = 3;
        data_8018f5a0->step_index = 0;
        ((Entity *)state)->field_c6 = 0;
        data_8018f5a0->substep_index = 0;
        ((Entity *)state)->field_c8 = 0;
        data_8018f5a0->pass_index = 0;
        ((Entity *)state)->field_cc = 0;
        ((Entity *)state)->field_ab = 0;
        ((Entity *)state)->table_index = 0;
        ((Entity *)state)->active_flag = 1;
        init_object_pools();
    }
}

void reset_hud_steps_or_call_fallback(GameState *state) {
    HudState *h;
    if (data_8016e68b == 0) {
        func_80010e60();
    } else {
        h = data_8018f5a0;
        h->step_index = 0;
        ((Entity *)state)->field_c6 = 0;
        h->substep_index = 0;
        ((Entity *)state)->active_flag = 1;
        ((Entity *)state)->field_c8 = 0;
        ((Entity *)state)->table_index = 0;
        ((Entity *)state)->field_2c = 0xff;
        player_left.field_01 = 0;
        player_right.field_01 = 0;
    }
}
