/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8011bc84(Slab172 *p) {
    u16 *cur = (u16 *)(((p->sequence->data_ptr >> 1) << 1) + p->stream_base);
    u16 *old;
    u16 *q;
    if (p->reload_flag != 0) {
        p->reload_flag = 0;
        old = data_8018437c[p->stream_index][0];
        if (old != cur) {
            shift_stream_second_cell_to_first(p);
            if (*cur != 0) {
                unpack_cell_list_into_stream_cells(p, cur);
            } else {
                drop_second_stream_cell_run(p, cur);
            }
            func_80119d88(p);
            q = get_sequence_data_address(p);
            if (*q != 0 && q != cur) {
                unpack_cell_list_into_stream_cells(p, q);
            } else {
                drop_second_stream_cell_run(p, q);
            }
        } else {
            func_80119d88(p);
            q = get_sequence_data_address(p);
            if (*q != 0 && q != old) {
                unpack_cell_list_into_stream_cells(p, q);
            } else {
                drop_second_stream_cell_run(p, q);
            }
        }
    } else {
        decode_blocks_divided_by_step_divisor(p);
    }
    build_sprite_quads_from_stream(p);
}
