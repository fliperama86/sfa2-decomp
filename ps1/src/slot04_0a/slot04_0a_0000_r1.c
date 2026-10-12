/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b5174_slot04_0a[];
extern u8 data_801b6e48_slot04_0a[];
extern u32 data_801c0450_slot04_0a[];

void func_801b0000_slot04_0a(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b5174_slot04_0a;
    obj->data_ptr_b = data_801b6e48_slot04_0a;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c0450_slot04_0a[i];
    }
    c = func_8011f32c();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0xa;
        c->table_index = obj->kind;
        c->parent = obj;
        obj->linked_object = (u32)c;
        *(u32 *)&c->pos_x_frac = *(u32 *)&obj->pos_x_frac;
        *(u32 *)&c->pos_y_frac = *(u32 *)&obj->pos_y_frac;
        c->variant_index = 0xff;
        c->facing_side = obj->facing_side;
        c->field_0e = obj->field_0e;
        c->field_0c = obj->field_0c;
        c->image_row = obj->image_row;
        c->sound_side = obj->side;
        c->image_row = obj->image_row;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
    }
    if (game_state.count_index == 0) {
        obj->active_state = 1;
        obj->pos_y = obj->pos_y - 0x30;
    }
}
