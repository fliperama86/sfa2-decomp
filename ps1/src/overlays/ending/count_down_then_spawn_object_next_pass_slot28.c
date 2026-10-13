/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_80043864_slot28[];
extern u8 data_8004231c_slot28[];
extern u8 data_800426b8_slot28[];
extern Object *data_80051b18_slot28[];

void count_down_then_spawn_object_next_pass_slot28(Object *obj) {
    HudState *h = screen_state;
    Object *p;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t == 0) {
        h->countdown_timer = 0x168;
        h->pass_index++;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x2f;
            p->ot_depth = 2;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->pos_x = 0xb8;
            p->pos_y = 0xf0;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_8004231c_slot28;
            p->data_ptr_b = data_800426b8_slot28;
            p->table_index = 0;
            p->field_01 = 1;
            p->image_row = 0;
            ((Slot28Obj *)p)->sequence_ptr = data_80043864_slot28;
            data_80051b18_slot28[0] = p;
        }
    }
}
