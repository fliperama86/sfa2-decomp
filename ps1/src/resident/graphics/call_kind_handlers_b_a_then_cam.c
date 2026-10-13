/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



void call_kind_handlers_b_a_then_cam(void) {
    call_kind_handler_and_copy_fields_b();
    call_kind_handler_and_copy_fields_a();
    call_cam_kind_handler_and_save_pos();
}

void call_kind_handlers_b_cam_then_a(void) {
    call_kind_handler_and_copy_fields_b();
    call_cam_kind_handler_and_save_pos();
    call_kind_handler_and_copy_fields_a();
}

void call_kind_handler_and_copy_fields_a(void) {
    u16 p, q;
    data_801723c0[game_state.kind_index](&background_layers);
    p = data_801aa566;
    q = data_801aa56a;
    data_801aa556 = p;
    data_801aa55a = q;
    data_801aa594 = data_801aa598;
    data_801aa594 += data_801aa58c * 2;
    data_801aa594 += data_801aa58e * 2;
    data_801aa56e = p;
    data_801aa572 = q;
}

void init_sprite_once_with_mode_y(Sprite *sprite) {
    init_sprite_position(sprite, 0x1c0, (game_state.field_0a == 6) << 8);
    if (sprite->pending != 0) {
        sprite->pending = 0;
        sprite->count++;
    }
}

void init_sprite_at_1c0_zero(Sprite *sprite) {
    init_sprite_counted(sprite, 0x1c0, 0);
}

void init_sprite_counted(Sprite *sprite, s16 x, s16 y) {
    init_sprite_position(sprite, x, y);
    if (sprite->pending != 0) {
        sprite->pending = 0;
        sprite->count++;
    }
}
