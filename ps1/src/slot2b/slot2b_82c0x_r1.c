/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079a2c_slot2b[];
extern s16 data_8007ef2c_slot2b;
void func_80078554_slot2b(Object *o);

void func_800782c0_slot2b(Object *o) {
    if ((game_state_second.field_65 | game_state_second.field_a8) != 0) {
        func_8011ffdc(o);
        return;
    }
    data_80079a2c_slot2b[o->field_06](o);
    func_8011ff74(o);
    func_8011ffdc(o);
}
