/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void init_strips_and_load_three_images_slot0f(void);
void tick_timer_and_build_two_tilemap_layers_slot0f(void);
void reset_hud_state_and_sound_by_arg_slot0f(Object *obj, int a);

void update_slot0f_button_or_timeout_exit_slot0f(GameState *state, Menu *menu) {
    if (((data_801a696a | data_801a6976) & 0x860) != 0) {
        if (((data_801a696a | data_801a6976) & 0x820) != 0) {
            data_8018f5a0->step_index = 0;
            data_8018f5a0->phase_index++;
        } else {
            reset_hud_state_and_sound_by_arg_slot0f((Object *)state, 1);
        }
        play_sound_at(0, 0, 0x205);
        stop_sep_by_word(0x200);
    } else {
        state->step_counter = (s16)state->step_counter - 1;
        if ((s16)state->step_counter == 0) {
            reset_hud_state_and_sound_by_arg_slot0f((Object *)state, 1);
        }
    }
    tick_timer_and_build_two_tilemap_layers_slot0f();
}
