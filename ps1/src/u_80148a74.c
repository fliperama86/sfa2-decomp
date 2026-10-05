/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80148a74(Object *object) {
    /* The unused local reproduces the stack frame of the original, which reserves the
       space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[24];
    Object *other = object->field_3c;

    if (other->field_0b != 0) {
        object->pos_x = other->pos_x - table_8017cd54[(s16)object->field_46].first;
        object->pos_y = other->pos_y - table_8017cd54[(s16)object->field_46].second;
    } else {
        object->pos_x = other->pos_x + table_8017cd54[(s16)object->field_46].first;
        object->pos_y = other->pos_y - table_8017cd54[(s16)object->field_46].second;
    }
    object->pos_x = object->pos_x + table_8017cdb4[object->field_48].first;
    object->pos_y = object->pos_y - table_8017cdb4[object->field_48].second;
}
