/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Object *pop_object_from_second_pool(void) {
    Object *o;
    Object *result;
    int n = data_801a4fec;
    if (n != 0) {
        o = data_801a68f0[n];
        data_801a4fec = n - 1;
        clear_anim_obj_stream_index((AnimObj *)o);
        result = o;
    } else {
        result = 0;
    }
    return result;
}
