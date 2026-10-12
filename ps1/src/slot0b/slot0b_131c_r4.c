/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801e7a2c_slot0b[];
extern SequenceStep *data_801e4cc0_slot0b[];
SlotCell *init_list_cells_tpage_clut_slot0b(Object *obj, SlotCell *cell, int arg, int base);
void call_step_on_anim_member_slot0b(Slot0bObj *obj);
void advance_cursor_frame_on_timer_slot0b(Slot0bCursor *c);
void set_slot0b_cursor_frame_slot0b(Slot0bCursor *c, Slot0bFrame *f);
void queue_pool_job_rect_1e7_from_cursor_slot0b(Slot0bCursor *c);

void func_801e1598_slot0b(Slot0bObj *obj, int arg) {
    u8 index = arg;
    int a;
    set_sequence_from_table_5((Object *)obj, index, data_801e4cc0_slot0b);
    a = func_8015bd0c(0, 0, 0x300, 0) & 0xffff;
    init_list_cells_tpage_clut_slot0b((Object *)obj, (SlotCell *)&data_801e7a2c_slot0b[obj->entry_index * 0x50], a, (u16)func_8015bdd4(0x140, 0x1e7));
}

void call_step_on_anim_member_slot0b(Slot0bObj *obj) {
    advance_cursor_frame_on_timer_slot0b(&obj->anim);
}

void advance_cursor_frame_on_timer_slot0b(Slot0bCursor *c) {
    Slot0bFrame *f;
    c->timer--;
    if (c->timer == 0) {
        f = c->cur + 1;
        if (c->loop < 0) {
            f = c->cur;
        }
        set_slot0b_cursor_frame_slot0b(c, f);
    }
}

void set_slot0b_cursor_frame_slot0b(Slot0bCursor *c, Slot0bFrame *f) {
    c->cur = f;
    c->timer = f->duration;
    c->loop = c->cur->loop;
    queue_pool_job_rect_1e7_from_cursor_slot0b(c);
}
