/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void decode_blocks_divided_by_step_divisor(AnimObj *a) {
    if (a->step_divisor < 0) {
        a->step_divisor = 0;
    }
    queue_compressed_block_jobs(a, (data_80183bb0[a->stream_index] - data_80183ad0[a->stream_index]) / (a->step_divisor + 1));
}
