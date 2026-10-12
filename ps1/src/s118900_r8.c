/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void release_table_index_cell_runs(AnimObj *a) {
    u16 *s0 = data_80183dc8[a->table_index];
    u16 *s1 = data_80183da0[a->table_index];
    u32 n = 2;
    do {
        free_slot_chain(*s1++, *s0);
        n++;
        *s0++ = 0;
    } while (n < 9);
}

void queue_block_jobs_for_stream_difference(AnimObj *a) {
    queue_compressed_block_jobs(a, data_80183bb0[a->stream_index] - data_80183ad0[a->stream_index]);
}
