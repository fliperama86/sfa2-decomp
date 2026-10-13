/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_8002d568_slot12;
extern Object *data_8002d56c_slot12;
extern int data_8002d578_slot12;
extern unsigned data_80028a98_slot12;
void load_and_store_palette_rows_slot12(void);
void spawn_block_with_saved_side_slot12(void);

void init_step_screen_and_spawn_blocks_slot12(void) {
    Rect r;
    Block172 *b;
    Object *o;
    int one;
    int cnt;
    if (game_state.field_44 & 0x80) {
        while (game_state.wait_flag != 0) {
            update_blocks();
            update_active_slab172_entries();
            queue_player_status_text_items();
            request_sound_command_and_switch_thread(1);
        }
        start_fade_mode_c01_and_wait();
        while (game_state.wait_flag != 0) {
            update_blocks();
            update_active_slab172_entries();
            queue_player_status_text_items();
            request_sound_command_and_switch_thread(1);
        }
    }
    kind_handlers_enabled = 1;
    screen_state->step_index = screen_state->step_index + 1;
    o = game_state.object;
    game_state.countdown_2 = 0x1f;
    game_state.field_d2 = 0x40;
    game_state.status_flags = 0;
    game_state.field_4f = 0;
    game_state.field_65 = 0;
    game_state.cam_x = 0;
    game_state.cam_y = 0;
    game_state.field_d4 = 0x100;
    game_state.cam_param_a = 0;
    game_state.cam_param_b = 0;
    game_state.field_da = 0x100;
    data_8002d56c_slot12 = o;
    data_8002d568_slot12 = &player_left + (o->side ^ 1);
    game_state.loop_mode = 0;
    set_menu_command_by_code(1, 0x607);
    while (menu_active_flag == 0 || game_state.wait_flag != 0) {
        update_blocks();
        update_active_slab172_entries();
        queue_player_status_text_items();
        request_sound_command_and_switch_thread(1);
    }
    r.x = 0x180;
    r.y = 0x100;
    r.w = 0x40;
    r.h = 0x100;
    func_80157fc4(&r, (u8 *)0x80043344);
    func_80157d9c(0);
    load_and_store_palette_rows_slot12();
    one = 1;
    data_80028a98_slot12 = one;
    b = pop_block_from_pool();
    if (b != 0) {
        b->active_flag = 1;
        b->handler_index = 0x26;
        b->block_mode = 0;
        if (game_state.field_44 & 0x80) {
            data_8002d578_slot12 = one;
            b = pop_block_from_pool();
            if (b != 0) {
                b->handler_index = 0x7b;
                b->active_flag = 1;
                b->block_mode = 1;
                b->clut_x = 0;
                b->clut_y = 0x1e0;
            }
            b = pop_block_from_pool();
            if (b != 0) {
                b->handler_index = 0x7b;
                b->block_mode = 2;
                b->active_flag = 1;
                b->clut_x = 0;
                b->clut_y = 0x1e0;
            }
            while (data_80028a98_slot12-- > 0) {
                update_blocks();
                update_active_slab172_entries();
                queue_player_status_text_items();
                request_sound_command_and_switch_thread(1);
            }
            spawn_block_with_saved_side_slot12();
        }
    }
    if (data_8002d578_slot12 != 0) {
        data_8002d56c_slot12->pos_x = box_margin[0] + 0xc0;
    } else {
        data_8002d568_slot12->pos_x = box_margin[0] + 0xc0;
    }
}
