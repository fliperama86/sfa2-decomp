/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_800797ec_slot00[];

void func_80076bdc_slot00(Object *o) {
    func_80137cc0(o);
}

void func_80076bfc_slot00(Object *o) {
    Config *config = game_state.config;

    if (config->pause_flag_a == 0 && config->pause_flag_b == 0) {
        data_800797ec_slot00[o->state_2](o);
    }
    func_8011ffdc(o);
}
