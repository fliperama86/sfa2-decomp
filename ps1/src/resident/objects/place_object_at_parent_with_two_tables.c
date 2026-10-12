/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void place_object_at_parent_with_two_tables(Object *object) {
    /* The unused local reproduces the stack frame of the original, which reserves the
       space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[24];
    Object *other = object->parent;

    if (other->facing_side != 0) {
        object->pos_x = other->pos_x - table_8017cd54[(s16)object->countdown].first;
        object->pos_y = other->pos_y - table_8017cd54[(s16)object->countdown].second;
    } else {
        object->pos_x = other->pos_x + table_8017cd54[(s16)object->countdown].first;
        object->pos_y = other->pos_y - table_8017cd54[(s16)object->countdown].second;
    }
    object->pos_x = object->pos_x + table_8017cdb4[object->variant_index].first;
    object->pos_y = object->pos_y - table_8017cdb4[object->variant_index].second;
}
