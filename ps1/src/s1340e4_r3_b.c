/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void call_kind_handler_and_copy_fields_b(void) {
    data_80172410[game_state.kind_index](&data_801aa5d4);
    data_801aa5fe = data_801aa5f6;
    data_801aa602 = data_801aa5fa;
}

void init_sprite_wide_and_consume_pending(Sprite *sprite) {
    init_sprite_position_wide(sprite, 0x1c0, 0);
    if (sprite->pending != 0) {
        sprite->pending = 0;
        sprite->count++;
        if (game_state.field_0a == 6) sprite->field_4a = 0x200;
    }
}

void init_sprite_3ff_and_count_pending(Sprite *sprite) {
    init_sprite_position_wide(sprite, 0x1c0, 0);
    if (sprite->pending != 0) {
        sprite->pending = 0;
        sprite->count++;
    }
}

void update_camera_if_active(Sprite *sprite) {
    if (sprite->active_flag != 0 && game_state.side_bits_2 == 0) {
        update_camera_x_from_player_span(sprite);
        update_cam_y_toward_players(sprite);
    }
}
