/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_8005146c_slot28[];
extern u8 data_80051528_slot28[];
extern BoxTables data_800516dc_slot28;
extern ObjectRef data_80051df0_slot28;

void spawn_objects_by_state_index_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    Object *p;
    int i = 0;
    switch (obj->state_index) {
    case 3:
        break;
    case 2:
    case 0x14:
        for (i = 0; i < 2; i++) {
            p = (Object *)pop_block_from_pool();
            if (p != 0) {
                p->active_count = 1;
                p->field_02 = 0xaf;
                p->data_ptr_a = data_8005146c_slot28;
                p->data_ptr_b = data_80051528_slot28;
                p->spawn_x = 0x60;
                p->spawn_y = 0x1e0;
                p->ot_depth = 4;
                p->table_index = i;
                p->field_90 = (void *)0x80078000;
                p->image_row = 0;
                p->box_tables = &data_800516dc_slot28;
            }
        }
        break;
    default:
        for (i = 0; i < 6; i++) {
            p = (Object *)pop_block_from_pool();
            if (p != 0) {
                p->active_count = 1;
                p->field_02 = 0xae;
                p->data_ptr_a = data_8005146c_slot28;
                p->data_ptr_b = data_80051528_slot28;
                p->spawn_x = 0x60;
                p->spawn_y = 0x1e0;
                p->ot_depth = 4;
                p->table_index = i;
                p->field_90 = (void *)0x80078000;
                p->image_row = 0;
                p->box_tables = &data_800516dc_slot28;
            }
        }
        break;
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0xb0;
        p->data_ptr_a = data_8005146c_slot28;
        p->data_ptr_b = data_80051528_slot28;
        p->pos_x = 0xb8;
        p->pos_y = 0xf30;
        p->spawn_x = 0x60;
        p->spawn_y = 0x1e0;
        p->ot_depth = 3;
        p->table_index = 0;
        p->field_90 = (void *)0x80078000;
        p->image_row = 0;
        p->box_tables = &data_800516dc_slot28;
        p->variant_index = 0;
    }
    data_80051df0_slot28.p = p;
}
