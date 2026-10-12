/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void clear_anim_obj_stream_index(AnimObj *a) {
    a->stream_index = 0;
}
