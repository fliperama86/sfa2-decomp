/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void clear_object_side_table_entries(Object *o) {
    int i = o->table_index;
    int j = o->parent->side;
    data_80185c84[j][i][0] = 0;
    data_80185cc4[j][i][0] = 0;
    data_80185ce4[j][i][0] = 0;
    data_80185c84[j][i][1] = 0;
    data_80185cc4[j][i][1] = 0;
    data_80185ce4[j][i][1] = 0;
}

u16 get_free_list_head(void) {
    return data_801846fc;
}

u16 alloc_slot_after(int a, u16 b) {
    int head;
    int cur;
    int i;
    if (data_80184700 == 0) {
        return 0xffff;
    }
    cur = (u16)a;
    head = data_801846fc;
    data_801846fc = data_80183c90[head];
    for (i = 0; i < b; i++) {
        cur = data_80183c90[cur];
    }
    data_80183c90[cur] = head;
    data_80184700--;
    return head;
}

void free_slot_chain(u16 a, u16 b) {
    int cur;
    u32 i;
    if (b != 0) {
        cur = a;
        for (i = 0; i < b - 1; i++) {
            cur = data_80183c90[cur];
        }
        data_80183c90[cur] = data_801846fc;
        data_801846fc = a;
        data_80184700 += b;
    }
}

void init_sprite_tables(void) {
    init_sprite_cell_buffers();
    clear_side_tables();
    init_poly_ft4_buffers();
    init_sprite_buffers();
}
