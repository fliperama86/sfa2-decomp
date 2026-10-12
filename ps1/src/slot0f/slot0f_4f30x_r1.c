/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_hud_state_or_phase_slot0f(Object *obj);
void spawn_screen_handler_blocks_slot0f(Object *obj);
void init_palette_buffers_from_table_slot0f(int a);
void func_800e5258_slot0f(void);
void call_image_job_with_index_0_slot0f(Object *obj);
void copy_rank_palette_to_two_buffers_slot0f(Object *obj);

void func_800e4f30_slot0f(Object *arg) {
    Object *obj;
    Rect r;
    Block172 *b;
    int one;
    data_8018f5a0->phase_index++;
    game_state.field_2bc = 0;
    obj = arg;
    start_fade_mode_1_and_wait();
    if (obj->field_01 == 0) {
        data_801903c2 = (s16)data_801903c2 - 1;
        if ((s16)data_801903c2 == 5) {
            advance_hud_state_or_phase_slot0f(obj);
            return;
        }
    } else {
        data_801903c2 = 0;
    }
    func_8011eb14();
    one = 1;
    func_8011eae4();
    data_80190568 = one;
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
    init_palette_buffers_from_table_slot0f(1);
    func_800e5258_slot0f();
    call_image_job_with_index_0_slot0f(obj);
    copy_rank_palette_to_two_buffers_slot0f(obj);
    b = pop_block_from_pool();
    if (b != 0) {
        b->block_mode = 3;
        b->active_flag = one;
        b->handler_index = 0x99;
    }
    func_80151020(0x606);
}
