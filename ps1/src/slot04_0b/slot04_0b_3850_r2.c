/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3a30_slot04_0b(Object *o) {
    Config *config = game_state.config;

    if ((config->field_a8 | config->field_65) == 0) {
        *(s32 *)&o->pos_x_frac += o->vel_x;
        *(s32 *)&o->pos_y_frac += o->vel_y;
        advance_sequence_checked(o);
        if (o->field_ac >= 0xc) {
            func_8011ffa8(o);
        } else {
            func_8011ff74(o);
        }
    }
    func_8011ffdc(o);
}
