/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Aliases of player_left.field_b2 (read as u8) and player_right.field_b2 (read as s16); the member width differs. */

void func_8012510c(void) {
    Object *object = &player_left;
    func_80125144(object);
    func_80125144(object + 1);
}
