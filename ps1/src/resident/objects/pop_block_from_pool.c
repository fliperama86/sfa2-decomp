/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


Block172 *pop_block_from_pool(void) {
    Block172 *b;
    Block172 *result;
    if (data_80197f10 != 0) {
        b = table_80197f20[data_80197f10];
        data_80197f10--;
        clear_anim_obj_stream_index(b);
        result = b;
    } else {
        result = 0;
    }
    return result;
}
