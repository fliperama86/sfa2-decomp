/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051890_slot28;

void func_800157cc_slot28(Object *obj) {
    if (game_state.field_f0 == 0) {
        data_8018f5a0->field_4e++;
        func_8011f240((Slab172 *)data_80051890_slot28.p);
    }
}
