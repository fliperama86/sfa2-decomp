/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Block172 data_801e2318_slot2a[4];
extern Block172 *data_801e25c8_slot2a[4];
extern u32 data_801e1bdc_slot2a[];
extern u16 *data_801e52c8_slot2a;

void init_palette_and_blocks_for_stage_slot2a(void) {
    Rect rect;
    u16 pal[16] = {0x0000, 0x7bde, 0x6b5a, 0x6318, 0x5ad6, 0x4a52, 0x4210, 0x39ce,
                   0x294a, 0x2108, 0x18c6, 0x111a, 0x111a, 0x111a, 0x111a, 0x0842};
    Block172 *p;
    Block172 *base;
    int a1;

    data_8018f5a0->pass_index++;
    game_state.state_2c_flag = 1;
    game_state.status_flags = 0;
    game_state.field_4f = 0;
    game_state.status_flags = 0xff;
    game_state.field_84 = 0xff;
    game_state.field_89 = 0;
    rect.x = 0x2c0;
    rect.y = 0x80;
    rect.w = 0x10;
    rect.h = 1;
    func_80157fc4(&rect, (u8 *)pal);
    func_80157d9c(0);
    a1 = 0x407;
    if (game_state.stage_index == 0xb) {
        a1 = 0x503;
    }
    game_state.loop_mode = 0;
    set_menu_command_by_code(1, a1);
    while (data_80190949 == 0 || game_state.wait_flag != 0) {
        select_buffers_and_run_first_setup();
        request_sound_command_and_switch_thread(1);
    }
    a1 = 0;
    if (game_state.field_80 < 6) {
        int t = game_state.mode & 1;
        a1 = t == 0;
    }
    base = data_801e2318_slot2a;
    p = base;
    if (p) {
        p->active_flag = 1;
        data_801e2318_slot2a[0].handler_index = 0x7f;
        data_801e2318_slot2a[0].block_mode = 0;
        data_801e2318_slot2a[0].shared_arg = a1;
        data_801e25c8_slot2a[0] = p;
    }
    p = base + 1;
    if (p) {
        p->active_flag = 1;
        data_801e2318_slot2a[1].handler_index = 0x7f;
        data_801e2318_slot2a[1].block_mode = 1;
        data_801e2318_slot2a[1].shared_arg = a1;
        data_801e25c8_slot2a[1] = p;
    }
    p = base + 2;
    if (p) {
        p->active_flag = 1;
        data_801e2318_slot2a[2].handler_index = 3;
        data_801e2318_slot2a[2].block_mode = 0;
        data_801e2318_slot2a[2].shared_arg = a1;
        data_801e25c8_slot2a[2] = p;
    }
    p = base + 3;
    if (p) {
        p->active_flag = 1;
        data_801e2318_slot2a[3].handler_index = 3;
        data_801e2318_slot2a[3].block_mode = 1;
        data_801e2318_slot2a[3].shared_arg = a1;
        data_801e25c8_slot2a[3] = p;
    }
    data_801e52c8_slot2a = (u16 *)data_801e1bdc_slot2a[game_state.stage_index];
}
