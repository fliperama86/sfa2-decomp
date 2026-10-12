/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3a30_slot04_0b(Object *o) {
    Config *config = game_state.config;

    if ((config->pause_flag_b | config->pause_flag_a) == 0) {
        *(s32 *)&o->pos_x_frac += o->vel_x;
        *(s32 *)&o->pos_y_frac += o->vel_y;
        advance_sequence_checked(o);
        if (o->vel_table_index >= 0xc) {
            func_8011ffa8(o);
        } else {
            func_8011ff74(o);
        }
    }
    func_8011ffdc(o);
}
