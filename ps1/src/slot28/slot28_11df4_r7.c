/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80022b50_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        set_menu_command_by_code(6, 3);
        start_fade_mode_1_and_wait();
        data_8018f5a0->pass_index += 1;
    }
}

void func_80022bb8_slot28(Object *obj) {
    if (game_state.wait_flag == 0) {
        data_8018f5a0->substep_index += 1;
    }
}
