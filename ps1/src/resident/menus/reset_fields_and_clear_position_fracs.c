/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void reset_fields_and_clear_position_fracs(ObjectView *a, Object *b, Object *c) {
    a->field_1e = 0x1c3;
    a->field_32 = 0;
    a->field_33 = 0;
    a->field_1d = 0;
    b->pos_x_frac = 0;
    b->pos_y_frac = 0;
    c->pos_x_frac = 0;
    c->pos_y_frac = 0;
}
