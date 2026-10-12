/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void wait_countdown_or_set_state_index_5(GameState *state) {
    Entity *entity = (Entity *)state;
    if (entity->a9_flag == 0) {
        if ((u8)is_pad_pressed_for_leader_bits_or_mode() == 0) {
            int v = entity->countdown_timer - 1;
            entity->countdown_timer = v;
            if ((s16)v >= 0) {
                call_handler_by_data_index();
                return;
            }
        }
        ((HudBig *)data_8018f5a0)->state_index = 5;
    } else {
        ((HudBig *)data_8018f5a0)->state_index++;
    }
}
