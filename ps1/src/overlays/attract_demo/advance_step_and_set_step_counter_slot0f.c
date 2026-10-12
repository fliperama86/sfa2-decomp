/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void init_strips_and_load_three_images_slot0f(void);
void tick_timer_and_build_two_tilemap_layers_slot0f(void);
void reset_hud_state_and_sound_by_arg_slot0f(Object *obj, int arg);

void advance_step_and_set_step_counter_slot0f(GameState *state, Menu *menu) {
    data_8018f5a0->step_index++;
    init_strips_and_load_three_images_slot0f();
    state->step_counter = 0x708;
}
