/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801b6608_slot04_09[];
extern u8 data_801ba1b0_slot04_09[];
extern u32 data_801c7ae0_slot04_09[];

void init_object_copy_table_and_spawn_linked_slot04_09(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    u32 *dst;
    u8 *p;
    u32 i;
    Object *c;

    dst = (u32 *)0x1f800100;
    p = (u8 *)&obj->linked_object;
    o->data_ptr_a = data_801b6608_slot04_09;
    o->data_ptr_b = data_801ba1b0_slot04_09;
    if (o->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c7ae0_slot04_09[i];
    }
    p[4] = 0;
    p[5] = 0;
    *(u32 *)p = 0;
    o->variant_index_2 = 0;
    o->step_count = 0;
    o->field_a3 = 0;
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 9;
        c->parent = o;
        *(u32 *)&o->linked_object_2 = (u32)c;
        *(u32 *)&c->pos_x_frac = *(u32 *)&o->pos_x_frac;
        *(u32 *)&c->pos_y_frac = *(u32 *)&o->pos_y_frac;
        c->facing_side = o->facing_side;
        c->field_0e = o->field_0e;
        c->field_0c = o->field_0c;
        c->image_row = o->image_row;
        c->table_index = o->kind;
        c->sound_side = o->side;
        c->spawn_x = o->spawn_x;
        c->spawn_y = o->spawn_y;
        c->field_90 = o->field_90;
        c->data_ptr_a = o->data_ptr_a;
        c->data_ptr_b = o->data_ptr_b;
    }
}
