/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012306c(GameState *g) {
    Object *o = g->focus_object;
    if (o->mode_flag_2 == 0) {
        func_8012304c((Entity *)g);
        if ((u8)func_80125394() != 0 || g->bcd_step_amount > g->bcd_amount) {
            data_8018f5a0->step_index_2++;
            g->countdown_timer_2 = 0x5a;
            o->score = add_bcd(o->score, g->bcd_amount << 8);
            g->bcd_amount = 0;
            func_80156094();
            play_sound_at(0, 0, 0x34d);
            func_80123504(g);
        } else {
            g->bcd_amount = func_80156018(g->bcd_amount, g->bcd_step_amount);
            o->score = add_bcd(o->score, g->bcd_step_amount << 8);
            func_80156094();
            if ((game_state.frame_counter & 3) == 0) {
                play_sound_at(0, 0, 0x34d);
            }
        }
    }
}
