/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_800797f4_slot00[];
void func_80077050_slot00(Object *obj);

void func_80076d88_slot00(Object *o) {
    Config *config = game_state.config;

    if (config->field_65 == 0 && config->field_a8 == 0) {
        data_800797f4_slot00[o->field_06](o);
    }
    func_8011ffdc(o);
}
