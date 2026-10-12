/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void update_menu_cd_state_after_sync(Menu *menu) {
    int v = func_8015c938(1, &menu->cd_result);
    if (v != 0) {
        if (v == 2) {
            if (!(menu->cd_result & 0x40) && func_8015c9c0(0x1b, &menu->cd_position, 0)) {
                menu->command_state = 2;
                if (menu->fade_mode == 0) {
                    set_cd_volume(0x55);
                }
            }
        } else {
            menu->command_state = 7;
        }
    }
}

void send_cd_command_nine_set_state_three(Menu *menu) {
    menu->active_flag = 1;
    if (menu->flag_a & menu->flag_b) {
        if (func_8015c938(1, 0)) {
            if (func_8015c9c0(9, 0, &menu->cd_result)) {
                menu->command_state = 3;
                if (menu->cd_result & 0x10) {
                    menu->field_08 = 1;
                }
            }
        }
    }
}

void send_cd_command_1b_set_state_two(Menu *menu) {
    if (func_8015c938(1, 0) && menu->active_flag != 0 && menu->flag_a == 0) {
        if (func_8015c9c0(0x1b, 0, &menu->cd_result)) {
            menu->flag_b = 0;
            menu->command_state = 2;
            if (menu->cd_result & 0x10) {
                menu->field_08 = 1;
            }
        }
    }
}

void pause_cd_and_clear_menu_command(Menu *menu) {
    if (func_8015c938(1, 0)) {
        if (func_8015c9c0(9, 0, 0)) {
            func_8016a730(0, 0, 0);
            set_cd_volume(0);
            menu->active_flag = 0;
            menu->command_state = 0;
        }
    }
}

void send_cd_command_15_set_state_one(Menu *menu) {
    if (func_8015c938(1, 0)) {
        if (func_8015c9c0(0x15, &menu->cd_position, 0)) {
            menu->command_state = 1;
        }
    }
}

void send_cd_command_0d_set_state_five(Menu *menu) {
    if (func_8015c938(1, 0)) {
        if (func_8015c9c0(0xd, &menu->cd_param, 0)) {
            menu->command_state = 5;
        }
    }
}

void send_cd_command_0e_set_state_six(Menu *menu) {
    if (func_8015c938(1, 0)) {
        u8 buf = 0xc8;
        if (func_8015c9c0(0xe, &buf, &menu->cd_result)) {
            menu->command_state = 6;
            if (menu->cd_result & 0x10) {
                menu->field_08 = 1;
            }
        }
    }
}

void update_cd_volume_fade(Menu *menu) {
    u8 vol = menu->volume;
    u8 step = menu->fade_step;
    if (menu->fade_mode == 1) {
        vol = vol + step;
        if (vol > 0x55) {
            vol = 0x55;
            menu->fade_mode = 0;
        }
    } else {
        vol = vol - step;
        if ((s8)vol < 4) {
            vol = 0;
            menu->fade_mode = 0;
            menu->command_state = 4;
        }
    }
    set_cd_volume(vol);
}
