/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80055f30_slot01;
extern u16 data_80055f34_slot01;
extern int data_80055f3c_slot01;
extern Pair data_80015a70_slot01[];
extern u16 data_80015ac0_slot01[];

void set_scroll_origin_from_tables_slot01(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    s16 *q = &game_state.field_d4;
    int n;
    int t;
    int u;
    obj->pos_x = -0x140;
    obj->pos_y = -0x100;
    *q = -0x100;
    n = data_80055f3c_slot01;
    t = -data_80015a70_slot01[n].first - data_80015ac0_slot01[n];
    game_state.field_d2 = t;
    data_80055f30_slot01 = t;
    u = data_80015a70_slot01[n].second;
    u -= 0x110;
    *q = u;
    data_80055f34_slot01 = u;
}

void spawn_three_linked_objects_slot01(Object *obj) {
    Object *p;
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0x2b;
        p->table_index = 2;
        p->pos_x = obj->pos_x + 0x100;
        p->pos_y = obj->pos_y;
        p->linked_object = (u32)obj;
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0x2b;
        p->table_index = 3;
        p->pos_x = obj->pos_x + 0x200;
        p->pos_y = obj->pos_y;
        p->linked_object = (u32)obj;
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0x2b;
        p->table_index = 4;
        p->pos_x = obj->pos_x + 0x300;
        p->pos_y = obj->pos_y;
        p->linked_object = (u32)obj;
    }
}
