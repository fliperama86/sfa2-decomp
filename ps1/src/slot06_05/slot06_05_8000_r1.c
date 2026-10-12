/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_first_layer_tile_map_slot06_05(Slot06Layer *layer);
void func_801e863c_slot06_05(Slot06Layer *layer);
void func_801e8a80_slot06_05(Slot06Layer *layer);
void noop_byte_pointer_slot06_05(u8 *p);
void update_layer_follow_main_layer_eighth_slot06_05(Slot06Layer *layer);
void func_801e8258_slot06_05(void);
void func_801e8668_slot06_05(void);
void func_801e8aac_slot06_05(void);
void noop_empty_function_3_slot06_05(void);
void draw_tile_layer_0_16px_slot06_05(void);
void draw_tile_layer_1_16px_slot06_05(void);
void draw_tilemap_layer_32px_9x13_slot06_05(void);
void noop_empty_function_2_slot06_05(void);

void draw_background_layers_by_flags_slot06_05(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        draw_tile_layer_0_16px_slot06_05();
    }
    if (*flags & 4) {
        draw_tile_layer_1_16px_slot06_05();
    }
    if (*flags & 8) {
        draw_tilemap_layer_32px_9x13_slot06_05();
    }
    if (*flags & 4) {
        noop_empty_function_2_slot06_05();
    }
}

void init_background_layers_slot06_05(void) {
    init_first_layer_tile_map_slot06_05((Slot06Layer *)data_801aa544);
    func_801e863c_slot06_05((Slot06Layer *)data_801aa5d4);
    func_801e8a80_slot06_05((Slot06Layer *)cam_obj);
    noop_byte_pointer_slot06_05(data_801904d8);
}

void func_801e80f0_slot06_05(void) {
    func_801e8258_slot06_05();
    func_801e8668_slot06_05();
    func_801e8aac_slot06_05();
    noop_empty_function_3_slot06_05();
}

void func_801e8128_slot06_05(Slot06Layer *layer) {
    if (layer->mode == 0) {
        init_sprite_counted((Sprite *)layer, 0x40, 0);
    } else if (layer->mode == 1) {
        update_layer_follow_main_layer_eighth_slot06_05(layer);
    }
}
