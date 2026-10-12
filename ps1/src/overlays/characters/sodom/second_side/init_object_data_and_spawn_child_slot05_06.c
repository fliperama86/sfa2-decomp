/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern u8 data_801ce790_slot05_06[];
extern u8 data_801d1054_slot05_06[];
extern u32 data_801dd184_slot05_06[];

void init_object_data_and_spawn_child_slot05_06(Object *obj) {
    u32 *dst;
    int i;
    Object *c;
    u8 s;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801ce790_slot05_06;
    obj->data_ptr_b = data_801d1054_slot05_06;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801dd184_slot05_06[i];
    }
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 6;
        c->table_index = 0;
        s = obj->facing_side;
        obj->linked_object = (u32)c;
        c->parent = obj;
        c->facing_side = s;
        c->sound_side = obj->side;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
    }
}
