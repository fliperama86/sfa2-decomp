/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80029324_slot27;
void call_handler_by_hud_step_index_2_2_slot27(void);

void load_slot27_screen_graphics_and_camera_slot27(void) {
    Rect r;
    s16 i;
    set_menu_command_one_and_wait_pending(0x307);
    game_state.field_6d = 0;
    clear_arrays_and_init_pools_and_sprite_tables();
    r.x = 0x380;
    data_80029324_slot27 = 0;
    data_8018db10 = 0;
    r.y = 0x100;
    r.w = 0x40;
    r.h = 0x100;
    func_80157fc4(&r, ((u8 *)0x800e5000));
    r.x = 0x40;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80157fc4(&r, ((u8 *)0x800ed800));
    load_image_from_stream((u8 *)0x800ee400);
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = ((u16 *)0x800e0c00)[i];
        data_801a27e4_rows[5][i] = ((u16 *)0x800e0c00)[i];
    }
    queue_image_upload_job(0, 0);
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[1][i] = ((u16 *)0x800e0000)[i];
        data_801a27e4_rows[6][i] = ((u16 *)0x800e0000)[i];
    }
    queue_image_upload_job(1, 1);
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = ((u16 *)0x800e0400)[i];
        data_801a27e4_rows[7][i] = ((u16 *)0x800e0400)[i];
    }
    queue_image_upload_job(2, 2);
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[3][i] = ((u16 *)0x800e0800)[i];
        data_801a27e4_rows[8][i] = ((u16 *)0x800e0800)[i];
    }
    queue_image_upload_job(3, 3);
    for (i = 0; i < 0xa0; i++) {
        data_801a27e4_rows[2][0x160 + i] = ((u16 *)0x800ed400)[i];
        data_801a27e4_rows[7][0x160 + i] = ((u16 *)0x800ed400)[i];
    }
    queue_image_upload_job(2, 2);
    data_801aa544[0].scroll_x = 0x248;
    ((u16 *)&data_801aa544[0].scroll_y)[1] = 0x100;
    data_801aa544[1].scroll_x = 0x148;
    ((u16 *)&data_801aa544[1].scroll_y)[1] = 0;
    data_801aa544[2].scroll_x = 0x48;
    ((u16 *)&data_801aa544[2].scroll_y)[1] = 0x700;
    game_state.cam_x = 0x248;
    game_state.cam_y = 0x100;
    game_state.field_d2 = 0x148;
    game_state.field_d4 = 0;
    game_state.cam_param_a = 0x48;
    game_state.cam_param_b = 0x700;
    game_state.field_da = 0;
    func_80157d9c(0);
    if (game_state.pause_flag != 0) {
        data_8018f5a0->substep_index++;
        game_state.pause_flag = 0;
        call_handler_by_hud_step_index_2_2_slot27();
    } else {
        HudState *h = data_8018f5a0;
        h->step_index_2++;
        h->pass_index = 0;
        h->field_54 = 0;
        game_state.countdown_2 = 0;
        game_state.timer_or_bits = 0;
        game_state.b0_flag = 0;
        game_state.selection_value = 0xff;
        game_state.b2_count = 0;
    }
}
