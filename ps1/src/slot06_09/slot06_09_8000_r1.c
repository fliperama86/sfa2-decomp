/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ea124_slot06_09[];
extern u16 data_801e9e78_slot06_09[];
extern s16 data_801e9e7a_slot06_09[];
extern u16 data_80190542;

void init_first_layer_tile_map_slot06_09(Slot06Layer *layer);
void init_layer_map_fields_main_slot06_09(Slot06Layer *layer);
void set_layer_tile_map_and_scale_f_slot06_09(Slot06Layer *layer);
void init_layer_map_fields_basic_slot06_09(Slot06Layer *layer);
void init_slot06_09_two_tile_groups_gray_slot06_09(void);
void init_slot06_09_tiles_gray_slot06_09(void);
void init_two_banks_of_91_gray_32x32_sprite_prims_slot06_09(void);
void init_two_banks_of_poly_ft4_slot06_09(void);
void draw_tile_map_layer_16px_slot06_09(void);
void draw_tile_layer_sprites_with_partial_row_slot06_09(void);
void draw_tilemap_layer_32px_7x13_slot06_09(void);
void draw_skewed_tile_layer_quads_slot06_09(void);
void init_layer_sprite_and_advance_mode_slot06_09(Slot06Layer *layer);
void update_layer_half_parallax_by_state_slot06_09(Slot06Layer *layer);
int scroll_layer_x_with_table_steps_slot06_09(Slot06Layer *layer);
void set_layer_pos_y_and_timer_from_tables_slot06_09(Slot06Layer *layer);

void draw_background_layers_by_flags_slot06_09(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_map_layer_16px_slot06_09();
    }
    if (*flags & 4) {
        draw_tile_layer_sprites_with_partial_row_slot06_09();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_7x13_slot06_09();
    }
    if (*flags & 4) {
        draw_skewed_tile_layer_quads_slot06_09();
    }
}

void init_four_layers_tile_maps_slot06_09(void) {
    init_first_layer_tile_map_slot06_09((Slot06Layer *)data_801aa544);
    init_layer_map_fields_main_slot06_09((Slot06Layer *)data_801aa5d4);
    set_layer_tile_map_and_scale_f_slot06_09((Slot06Layer *)cam_obj);
    init_layer_map_fields_basic_slot06_09((Slot06Layer *)data_801904d8);
}

void init_slot_tile_groups_and_poly_buffers_gray_g_slot06_09(void) {
    init_slot06_09_two_tile_groups_gray_slot06_09();
    init_slot06_09_tiles_gray_slot06_09();
    init_two_banks_of_91_gray_32x32_sprite_prims_slot06_09();
    init_two_banks_of_poly_ft4_slot06_09();
}

void call_layer_handler_by_mode_slot06_09(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_layer_sprite_and_advance_mode_slot06_09(layer);
    } else if (layer->mode == 1) {
        update_layer_half_parallax_by_state_slot06_09(layer);
    }
}

void init_layer_sprite_and_advance_mode_slot06_09(Slot06Layer *layer) {
    init_sprite_position((Sprite *)layer, 0xc0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->table_index = 0;
        layer->mode++;
        set_layer_pos_y_and_timer_from_tables_slot06_09(layer);
    }
}

void update_layer_half_parallax_by_state_slot06_09(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->state_index == 0) {
        scroll_layer_x_with_table_steps_slot06_09(layer);
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->ref_x;
    d += data_80190542;
    d /= 2;
    d += layer->ref_x;
    d += layer->pos_x_int;
    layer->field_22 = d;
    e = l2->field_26;
    e -= l2->field_0e;
    e /= 2;
    e += layer->field_0e;
    e += layer->pos_y_int;
    layer->field_26 = e;
}

/* Declared int and returns nothing. With void this compiler fills a delay slot with a write to v0 and the function is 4 bytes short. The form is compatible with the original's bytes; what the original source declared is not known. */
int scroll_layer_x_with_table_steps_slot06_09(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        *(s32 *)&layer->pos_x += 0x3000;
        layer->countdown_timer -= 1;
        if (layer->countdown_timer == 0) {
            layer->table_index = (layer->table_index + 2) & 0x1e;
            set_layer_pos_y_and_timer_from_tables_slot06_09(layer);
        }
        if ((s16)layer->pos_x_int >= 0x3a0) {
            *(s32 *)&layer->pos_x = 0x3a00000;
            *(s32 *)&layer->pos_y = 0;
            layer->state_index++;
        }
    }
}

void set_layer_pos_y_and_timer_from_tables_slot06_09(Slot06Layer *layer) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u16 unused[4];

    layer->pos_y_int = data_801e9e78_slot06_09[layer->table_index];
    layer->countdown_timer = data_801e9e7a_slot06_09[layer->table_index];
}

void init_first_layer_tile_map_slot06_09(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ea124_slot06_09;
    layer->data_table = data_801ea124_slot06_09;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}
