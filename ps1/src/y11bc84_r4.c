/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void reload_cell_history_then_build_cell_prims(Slab172 *p) {
    u16 *cur = (u16 *)(((p->sequence->data_ptr >> 1) << 1) + p->stream_base);
    u16 *q;
    if (p->reload_flag != 0 || data_80183d74[p->field_02][0] != p->stream_key) {
        p->reload_flag = 0;
        if (data_80183dec[p->field_02][0] != (u32)cur || data_80183d74[p->field_02][0] != p->stream_key) {
            shift_history_tables_down(p);
            if (*cur != 0) {
                unpack_cell_list_with_history(p, cur);
            } else {
                push_front_of_cell_history(p, cur);
            }
            queue_block_jobs_for_stream_difference(p);
            q = get_sequence_data_address(p);
            if (*q != 0 && q != cur) {
                unpack_cell_list_with_history(p, q);
            } else {
                push_front_of_cell_history(p, q);
            }
        } else {
            queue_block_jobs_for_stream_difference(p);
            q = get_sequence_data_address(p);
            if (*q != 0 && q != cur) {
                unpack_cell_list_with_history(p, q);
            } else {
                push_front_of_cell_history(p, q);
            }
        }
    } else {
        decode_blocks_divided_by_step_divisor(p);
    }
    build_cell_prim_list_from_history((Object *)p, 0, 0, p);
    fill_cell_record_rects_from_history((Object *)p, 0, 0, p);
}
