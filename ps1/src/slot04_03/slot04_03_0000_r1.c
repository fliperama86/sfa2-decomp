/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b487c_slot04_03[];
extern u8 data_801b661c_slot04_03[];
extern u32 data_801c0460_slot04_03[];

void init_object_tables_and_spawn_child_slot04_03(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b487c_slot04_03;
    obj->data_ptr_b = data_801b661c_slot04_03;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c0460_slot04_03[i];
    }
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 3;
        c->table_index = obj->kind;
        c->parent = obj;
        obj->linked_object = (u32)c;
        c->sound_side = obj->sound_side;
        c->image_row = obj->image_row;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
    }
}
