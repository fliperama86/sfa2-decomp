/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void call_hud_handler_by_state_index_2(GameState *state) {
    table_8016e7e8[((HudBig *)data_8018f5a0)->state_index](state);
    call_input_step_handler();
}

void advance_state_by_mode_flag_a(GameState *state) {
    Entity *entity = (Entity *)state;
    if (entity->mode_flag_a == 0) {
        ((HudBig *)data_8018f5a0)->state_index++;
        entity->countdown_timer = 0x1e;
        func_80156084();
        call_handler_by_data_index();
        set_selected_value_by_input_threshold(entity);
    } else {
        ((HudBig *)data_8018f5a0)->state_index = 5;
        entity->countdown_timer = 0xa0;
        finish_countdown_and_start_next_round(state);
    }
}

void set_selected_value_by_input_threshold(Entity *entity) {
    if (entity->threshold_input < 0x500) {
        entity->selected_value = 4;
    } else {
        entity->selected_value = 0x100;
    }
}
