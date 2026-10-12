/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b52b4_slot04_07[];
extern u8 data_801b7630_slot04_07[];
extern u32 data_801c1d98_slot04_07[];

void init_object_copy_scratch_and_spawn_child_slot04_07(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b52b4_slot04_07;
    obj->data_ptr_b = data_801b7630_slot04_07;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c1d98_slot04_07[i];
    }
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 7;
        c->parent = obj;
        obj->linked_object = (u32)c;
        c->table_index = obj->kind;
        c->sound_side = obj->side;
        c->image_row = obj->image_row;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
    }
    if (game_state.count_index == 0) {
        if (obj->side == 0) {
            obj->pos_x = 0x1a0;
        } else {
            obj->pos_x = 0x380;
        }
        obj->pos_y = obj->pos_y - 0x38;
    }
}
