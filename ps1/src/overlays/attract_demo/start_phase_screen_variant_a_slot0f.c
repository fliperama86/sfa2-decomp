/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void advance_hud_state_or_phase_slot0f(Object *obj);
void spawn_screen_handler_blocks_slot0f(Object *obj);
void init_palette_buffers_from_table_slot0f(int a);
void fill_and_copy_20x16_halfword_tables_slot0f(void);
void call_image_job_with_index_0_slot0f(Object *obj);
void copy_rank_palette_to_two_buffers_slot0f(Object *obj);

void start_phase_screen_variant_a_slot0f(Object *arg) {
    Object *obj;
    Rect r;
    Block172 *b;
    int one;
    screen_state->phase_index++;
    game_state.field_2bc = 0;
    obj = arg;
    start_fade_mode_1_and_wait();
    if (obj->field_01 == 0) {
        data_801903c0 = (s16)data_801903c0 - 1;
        if ((s16)data_801903c0 == 5) {
            advance_hud_state_or_phase_slot0f(obj);
            return;
        }
    } else {
        data_801903c0 = 0;
    }
    one = 1;
    kind_handlers_enabled = one;
    r.x = 0x280;
    r.y = 0;
    r.w = 0x40;
    r.h = 0x100;
    func_80157fc4(&r, (u8 *)0x800b7000);
    func_80157d9c(0);
    r.x = 0x2c0;
    r.y = 0;
    r.w = 0x40;
    r.h = 0x100;
    func_80157fc4(&r, (u8 *)0x800bf000);
    func_80157d9c(0);
    spawn_screen_handler_blocks_slot0f(obj);
    init_palette_buffers_from_table_slot0f(0);
    fill_and_copy_20x16_halfword_tables_slot0f();
    call_image_job_with_index_0_slot0f(obj);
    copy_rank_palette_to_two_buffers_slot0f(obj);
    queue_image_upload_job(0, 6);
    b = pop_block_from_pool();
    if (b != 0) {
        b->block_mode = 3;
        b->active_flag = one;
        b->handler_index = 0x99;
    }
    set_menu_command_one_and_wait_pending(0x606);
}
