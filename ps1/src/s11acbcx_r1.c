/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

void update_bar_entry_and_add_prim(Object *object);
void func_801452ec(Object *object);

/* What it does (inferred, not an original name): updates the 40 records of the table data_801a89f4 (172 bytes each),
 * one by one, using the byte data_8019045c as the loop counter (it is 40 when the function ends). A record is skipped
 * unless its active_flag and enabled_flag are not 0. Then:
 *   - field_02 is 3, 0xb, 0x14, 0x27 or 0x6d and type_code is not 0x20: type 3, 0xb or 0x14 calls build_block_primitives, 0x6d
 *     calls update_bar_entry_and_add_prim, 0x27 with enabled_flag equal to 1 calls func_801452ec; 0x27 with another enabled_flag calls
 *     func_8011bc84. Each of these records is then done.
 *   - otherwise, handler_gate equal to 4: when data_80190568 is not 0, the stage handler selected by game_state.kind_index
 *     (0 to 18) is called with the record (a value outside 0 to 18 calls none);
 *   - otherwise, field_02 equal to 0x16 or 0x18 with type_code equal to 0x20: the parts at offsets 0x28 and 0x2c, the
 *     record itself, and the parts at 0x30 and 0x34 are passed to func_8011bc84 in that order; a part is passed only
 *     when its pointer is not null and its enabled_flag is not 0, and gets the record's copied_byte first;
 *   - otherwise func_8011bc84 is called with the record.
 */
void draw_all_block172_entries(void) {
    u8 *z = (u8 *)data_8019045c;
    u8 *i;
    int idx;
    int off;
    int t;
    *z = 0;
    do {
        /* Stand-in, not recovered source: the term t that is ORed to active_flag below, which is 0 for every value of the counter byte, reproduces the original's order of instructions. Without it this function differs from the original in 4 instruction slots. */
        t = ((u32)(unsigned long long)&data_801a89f4 & ((u32)*z >> 8));
        /* i is set here, at the top of the loop body; set before the loop it differs in 7 instruction slots. */
        i = (u8 *)data_8019045c;
        if (((data_801a89f4[*z].active_flag | t) != 0) && (data_801a89f4[*z].enabled_flag != 0)) {
            if (((((data_801a89f4[*z].field_02 == 3) || (data_801a89f4[*z].field_02 == 0xb)) ||
                  (data_801a89f4[*z].field_02 == 0x14)) ||
                 (data_801a89f4[*z].field_02 == 0x27)) ||
                (data_801a89f4[*z].field_02 == 0x6d)) {
                /* idx and off are locals: without off (idx * 172 written at each call) this function differs from the
                   original in 24 instruction slots, without idx (reading *i) in 11; the three calls with &data_801a89f4[idx] in place
                   of the byte address plus off differ in 24 (2292 bytes). */
                idx = *(u8 *)data_8019045c;
                off = idx * 172;
                if (data_801a89f4[idx].type_code != 0x20) {
                    if (((data_801a89f4[idx].field_02 == 3) || (data_801a89f4[idx].field_02 == 0xb)) ||
                        (data_801a89f4[idx].field_02 == 0x14)) {
                        build_block_primitives((Block172 *)(((u8 *)data_801a89f4) + off));
                    } else if (data_801a89f4[idx].field_02 == 0x6d) {
                        update_bar_entry_and_add_prim((Object *)(((u8 *)data_801a89f4) + off));
                    } else if ((data_801a89f4[idx].field_02 == 0x27) && (data_801a89f4[idx].enabled_flag == 1)) {
                        func_801452ec((Object *)(((u8 *)data_801a89f4) + off));
                    } else {
                        func_8011bc84(&data_801a89f4[*(u8 *)data_8019045c]);
                    }
                    goto next;
                }
            }
            if (data_801a89f4[*i].handler_gate == 4) {
                if (data_80190568 != 0) {
                    if (game_state.kind_index == 0) {
                        func_801e9080(&data_801a89f4[*i]);
                    }
                    if (game_state.kind_index == 1) {
                        func_801e9f90(&data_801a89f4[*i]);
                    }
                    if (game_state.kind_index == 2) {
                        func_801e99c8(&data_801a89f4[*i]);
                    }
                    if (game_state.kind_index == 3) {
                        func_801e9d04(&data_801a89f4[*i]);
                    }
                    if (game_state.kind_index == 4) {
                        func_801e99b4(&data_801a89f4[*i]);
                    }
                    if (game_state.kind_index == 5) {
                        func_801e8dc8(&data_801a89f4[*i]);
                    }
                    if (game_state.kind_index == 6) {
                        func_801e9798(&data_801a89f4[*i]);
                    }
                    if (game_state.kind_index == 7) {
                        func_801e9970(&data_801a89f4[*i]);
                    }
                    if (game_state.kind_index == 8) {
                        func_801e9b54(&data_801a89f4[*i]);
                    }
                    if (game_state.kind_index == 9) {
                        func_801e9840(&data_801a89f4[*(u8 *)data_8019045c]);
                    }
                    if (game_state.kind_index == 10) {
                        func_801e8df0(&data_801a89f4[*(u8 *)data_8019045c]);
                    }
                    if (game_state.kind_index == 11) {
                        func_801e96fc(&data_801a89f4[*(u8 *)data_8019045c]);
                    }
                    if (game_state.kind_index == 12) {
                        func_801ea640(&data_801a89f4[*(u8 *)data_8019045c]);
                    }
                    if (game_state.kind_index == 13) {
                        func_801e9f20(&data_801a89f4[*(u8 *)data_8019045c]);
                    }
                    if (game_state.kind_index == 14) {
                        func_801e9d14(&data_801a89f4[*(u8 *)data_8019045c]);
                    }
                    if (game_state.kind_index == 15) {
                        func_801e9bb0(&data_801a89f4[*(u8 *)data_8019045c]);
                    }
                    if (game_state.kind_index == 16) {
                        func_801e9ef4(&data_801a89f4[*(u8 *)data_8019045c]);
                    }
                    if (game_state.kind_index == 17) {
                        func_801e8fec(&data_801a89f4[*(u8 *)data_8019045c]);
                    }
                    if (game_state.kind_index == 18) {
                        func_801e9114(&data_801a89f4[*(u8 *)data_8019045c]);
                    }
                }
            } else if (((data_801a89f4[*i].field_02 == 0x16) || (data_801a89f4[*i].field_02 == 0x18)) &&
                       (data_801a89f4[*i].type_code == 0x20)) {
                Slab172 *q;
                q = (Slab172 *)data_801a89f4[*i].linked_entry_0;
                if ((q != 0) && (q->enabled_flag != 0)) {
                    q->copied_byte = data_801a89f4[*i].copied_byte;
                    func_8011bc84(q);
                }
                q = (Slab172 *)data_801a89f4[*(u8 *)data_8019045c].linked_entry_1;
                if ((q != 0) && (q->enabled_flag != 0)) {
                    q->copied_byte = data_801a89f4[*(u8 *)data_8019045c].copied_byte;
                    func_8011bc84(q);
                }
                func_8011bc84(&data_801a89f4[*i]);
                q = (Slab172 *)data_801a89f4[*i].linked_entry_2;
                if ((q != 0) && (q->enabled_flag != 0)) {
                    q->copied_byte = data_801a89f4[*i].copied_byte;
                    func_8011bc84(q);
                }
                q = (Slab172 *)data_801a89f4[*(u8 *)data_8019045c].linked_entry_3;
                if ((q != 0) && (q->enabled_flag != 0)) {
                    q->copied_byte = data_801a89f4[*(u8 *)data_8019045c].copied_byte;
                    func_8011bc84(q);
                }
            } else {
                func_8011bc84(&data_801a89f4[*(u8 *)data_8019045c]);
            }
        }
    next:
        (*i)++;
        /* Stand-in, not recovered source: the local z set to i below, which is only a second name of the counter's address, reproduces the original's choice of registers. Without it this function differs from the original in 15 instruction slots. */
        z = i;
    } while ((*i) < 40);
}
