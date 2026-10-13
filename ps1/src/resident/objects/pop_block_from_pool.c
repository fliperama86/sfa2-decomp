/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


Block172 *pop_block_from_pool(void) {
    Block172 *b;
    Block172 *result;
    if (free_block_count != 0) {
        b = table_80197f20[free_block_count];
        free_block_count--;
        clear_anim_obj_stream_index(b);
        result = b;
    } else {
        result = 0;
    }
    return result;
}
