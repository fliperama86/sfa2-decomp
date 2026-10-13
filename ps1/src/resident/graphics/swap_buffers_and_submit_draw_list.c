/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void swap_buffers_and_submit_draw_list(void) {
    int idx;
    int i;
    char *old;
    char *buf;
    Pooled *p;
    void *s;
    idx = draw_buffer_index ^ 1;
    old = current_ordering_table;
    buf = (char *)0x801fc000 + idx * 0xe8;
    draw_buffer_index = idx;
    current_ordering_table = buf;
    func_80158470(buf + 0x78);
    func_80158374((unsigned *)((char *)current_ordering_table + 0x8c));
    for (i = 0; i < 0x100; i++) {
        p = &data_8018e598[i];
        if (p->active_flag != 0) {
            func_80157fc4((char *)p + 8, p->base_address);
            release_pooled_to_stack_a(p);
        }
    }
    if (game_state.field_31 == 0) {
        update_palette_scale_by_player_flags(&game_state);
    }
    for (i = 0; i < 0x10; i++) {
        p = &data_8018f5e4[i];
        if (p->active_flag != 0) {
            func_80157fc4((char *)p + 8, p->base_address);
            release_pooled_to_stack_b(p);
        }
    }
    if (data_8018daf4[0] != 0) {
        copy_image_rows_and_load_image(&player_left);
    }
    if (data_8018daf4[1] != 0) {
        copy_image_rows_and_load_image(&player_right);
    }
    s = old + 0x74;
    data_8018db14 = 0;
    tile_sprite_count = 0;
    compact_linked_cell_runs(s);
    func_80158300(s);
    func_80158208(current_ordering_table, 0x1e);
    data_801901be = 0;
}
