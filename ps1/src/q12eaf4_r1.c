/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* One int local holds the side and then the shifted mask. */
void func_8012eaf4(Object *object) {
    int m = object->side;
    m = 1 << m;
    game_state.field_4b |= m;
}
