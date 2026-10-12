/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void clear_field_and_increment_step_count(Actor *a) {
    a->field_02 = 0;
    a->step_count++;
}
