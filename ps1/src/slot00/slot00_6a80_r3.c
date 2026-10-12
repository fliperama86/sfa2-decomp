/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_800797f4_slot00[];
void func_80077050_slot00(Object *obj);

void func_80076d88_slot00(Object *o) {
    Config *config = game_state.config;

    if (config->pause_flag_a == 0 && config->pause_flag_b == 0) {
        data_800797f4_slot00[o->state_2](o);
    }
    func_8011ffdc(o);
}
