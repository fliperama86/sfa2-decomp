/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void select_kind_for_second_player(Select *sel) {
    Object *first = &player_left;
    Object *second = first + 1;
    Object *tmp;
    u8 flag;
    s16 i;
    u8 j;
    u32 mask;
    int flags;

    if (sel->mode == 3) {
        if (sel->order_mode == 3) {
            if (sel->field_71 == 0) {
                tmp = first;
                first = second;
                second = tmp;
            }
        } else if (sel->order_mode & 1) {
            tmp = first;
            first = second;
            second = tmp;
        }
    } else {
        if (!(sel->mode & 1)) {
            tmp = first;
            first = second;
            second = tmp;
        }
        load_select_entries(sel, first, second);
        i = -1;
        flags = sel->excluded_mask;
    again:
        i++;
        flag = sel->entries[i];
        if (!(i < sel->index_limit)) {
            goto out;
        }
        j = 0;
        mask = 1;
        for (; j < flag; j++) {
            mask <<= 1;
        }
        if (mask & flags) {
            goto again;
        }
    out:
        sel->selected_index = i;
        second->kind = flag;
    }
    sel->kind_value = (u8)get_table_entry(second->kind);
    set_kind_value_when_index_at_limit(sel, first);
}
