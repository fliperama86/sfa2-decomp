/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b4e04_slot04_04[];
extern u8 data_801b7384_slot04_04[];
extern u32 data_801c40b0_slot04_04[];

void func_801b0000_slot04_04(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b4e04_slot04_04;
    obj->data_ptr_b = data_801b7384_slot04_04;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x29; i++) {
        dst[i] = data_801c40b0_slot04_04[i];
    }
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 4;
        c->table_index = 0;
        c->parent = obj;
        c->field_0c = obj->field_0c;
        c->image_row = obj->image_row;
        c->field_0e = obj->field_0e;
        c->sound_side = obj->sound_side;
        c->state_3 = obj->kind;
        *(u32 *)&obj->linked_object_2 = (u32)c;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
    }
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 4;
        c->table_index = 1;
        c->parent = obj;
        c->field_0c = obj->field_0c;
        c->image_row = obj->image_row;
        c->field_0e = obj->field_0e;
        c->state_3 = obj->kind;
        c->sound_side = obj->sound_side;
        obj->linked_object = (u32)c;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
    }
}
