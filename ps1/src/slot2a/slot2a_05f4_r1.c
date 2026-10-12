/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

typedef void (*SlotFn)(Object *);

extern u8 data_801e25d8_slot2a[];
extern SlotFn table_801e225c_slot2a[];
extern u8 data_801e25dc_slot2a[];
extern u16 *data_801e52c8_slot2a;
void update_state_1_and_draw_sprites_slot2a(Object *obj);
void set_sequence_data_ptr_and_sequence_slot2a(Object *obj, u8 a);
void build_and_link_sprite_set_prims_slot2a(Object *obj, SpritePrim *p);
void init_poly_cells_from_sequence_list_slot2a(Object *obj, SlotCell *cell, s16 n);
void build_sprite_arrays_from_spawn_table_slot2a(Object *obj);
void link_sprite_array_into_ordering_table_slot2a(Object *obj);

void func_801e05f4_slot2a(Object *obj) {
    u8 t;
    obj->state_1 = 2;
    obj->field_01 = 0;
    obj->field_0e = 0;
    obj->ot_depth = 0;
    obj->field_0f = 0;
    obj->scroll_factor = 0;
    obj->state_0 = obj->state_0 + 1;
    data_801e25d8_slot2a[obj->table_index] = 0;
    obj->pos_y = 0x40;
    if (obj->table_index != 0) {
        obj->pos_y = 0xa0;
    }
    obj->pos_x = 0x30;
    t = obj->active_state ^ obj->table_index;
    obj->facing_side = t;
    if (t != 0) {
        obj->pos_x = 0x140;
    }
    build_sprite_arrays_from_spawn_table_slot2a(obj);
    update_state_1_and_draw_sprites_slot2a(obj);
}

void update_state_1_and_draw_sprites_slot2a(Object *obj) {
    table_801e225c_slot2a[obj->state_1](obj);
    if (data_801e25d8_slot2a[obj->table_index] != 0) {
        build_and_link_sprite_set_prims_slot2a(obj, (SpritePrim *)(data_801e25dc_slot2a + obj->table_index * 0x500 + data_801a27d0 * 0x280));
    }
    link_sprite_array_into_ordering_table_slot2a(obj);
}

void select_parent_player_and_init_cells_slot2a(Object *obj) {
    obj->state_1++;
    if (obj->active_state != (*data_801e52c8_slot2a >> 8)) {
        obj->parent = &player_right;
    } else {
        obj->parent = &player_left;
    }
    set_sequence_data_ptr_and_sequence_slot2a(obj, obj->parent->kind);
    init_poly_cells_from_sequence_list_slot2a(obj, (SlotCell *)(data_801e25dc_slot2a + obj->table_index * 0x500), 0xb);
    init_poly_cells_from_sequence_list_slot2a(obj, (SlotCell *)(data_801e25dc_slot2a + 0x280 + obj->table_index * 0x500), 0xb);
    data_801e25d8_slot2a[obj->table_index] = 1;
}
