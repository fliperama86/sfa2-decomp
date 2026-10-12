/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_hud_phase_index_and_reset_slot12(Object *o);
void spawn_pool_blocks_or_fail_slot12(Object *obj);
void init_palette_buffers_from_table_slot12(int a);
void fill_and_copy_20x16_halfword_tables_slot12(void);
void call_image_job_with_index_0_slot12(Object *obj);
void copy_rank_palette_to_two_buffers_slot12(Object *obj);

void advance_right_rank_and_reload_screen_slot12(Object *obj) {
    Rect rect;
    Object *p;
    HudState *h = data_8018f5a0;

    h->step_index++;
    game_state.field_2bc = 0;
    game_state.right_rank_index--;
    if (game_state.right_rank_index == 5) {
        advance_hud_phase_index_and_reset_slot12(obj);
    } else {
        start_fade_mode_1_and_wait();
        set_menu_command_one_and_wait_pending(0x606);
        clear_players_arrays_state_and_init_pools();
        clear_arrays_and_init_pools_and_sprite_tables();
        init_block172_pool();
        data_80190568 = 1;
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x40;
        rect.h = 0x100;
        func_80157fc4(&rect, (u8 *)0x800b1000);
        func_80157d9c(0);
        rect.x = 0x2c0;
        rect.y = 0;
        rect.w = 0x40;
        rect.h = 0x100;
        func_80157fc4(&rect, (u8 *)0x800b9000);
        func_80157d9c(0);
        spawn_pool_blocks_or_fail_slot12(obj);
        init_palette_buffers_from_table_slot12(1);
        fill_and_copy_20x16_halfword_tables_slot12();
        call_image_job_with_index_0_slot12(obj);
        copy_rank_palette_to_two_buffers_slot12(obj);
        queue_image_upload_job(0, 6);
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->table_index = 3;
            p->active_count = 1;
            p->field_02 = 0xa4;
        }
    }
}
