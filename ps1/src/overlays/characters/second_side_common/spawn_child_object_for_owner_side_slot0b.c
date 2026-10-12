/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801e46b0_slot0b[];
extern u8 data_801e4728_slot0b[];
void queue_pooled_clut_upload_job_a_slot0b(void);

void spawn_child_object_for_owner_side_slot0b(ModObj *obj) {
    Object *p = obj->owner_object;
    ModObj *c = (ModObj *)pop_object_from_second_pool();
    u8 side;
    if (c != 0) {
        c->active_flag = 1;
        c->init_param = 0x1c;
        c->state_index = 0;
        side = p->side;
        c->field_66 = 0;
        c->table_ptr = (s32)data_801e46b0_slot0b;
        c->field_4c = 0;
        c->field_50 = 0;
        c->field_0b = 0;
        c->owner_object = (Object *)obj;
        c->side = side;
        obj->child = c;
    }
    if (p->side != 1) {
        queue_pooled_clut_upload_job_a_slot0b();
    }
}

void queue_pooled_clut_upload_job_a_slot0b(void) {
    ModPool *p = (ModPool *)alloc_pooled_b();
    Rect r;
    if (p != 0) {
        r.x = 0x140;
        r.y = 0x1e6;
        r.w = 0x10;
        r.h = 1;
        p->active_flag = 1;
        p->init_param = 2;
        p->rect = r;
        p->table_ptr = (u32)data_801e4728_slot0b;
    }
}
