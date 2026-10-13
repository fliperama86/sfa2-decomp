/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80055ea4_slot01;
extern int data_80055f3c_slot01;
extern char data_800100a0_slot01[];
char *strcat(char *dst, const char *src);

void format_hex_digits_eight_chars_slot01(int value, char *dst) {
    u8 digits[8];
    char *out = dst;
    int started = 0;
    int i;
    for (i = 0; i < 8; i++) {
        digits[i] = value % 16;
        value /= 16;
    }
    for (i = 7; i >= 0; i--) {
        if (started) {
            *out++ = digits[i] + 0x30;
        } else if (digits[i] == 0) {
            *out++ = 0x20;
        } else {
            *out++ = digits[i] + 0x30;
            started = 1;
        }
    }
    *out = 0;
    strcat(dst, data_800100a0_slot01);
}

void reset_hud_state_and_restore_kind_index_slot01(GameState *g) {
    HudState *h;
    set_menu_command_by_code(6, 2);
    game_state.kind_index = *(u16 *)&data_80055f3c_slot01;
    h = screen_state;
    kind_handlers_enabled = 0;
    data_80055ea4_slot01 = 0;
    h->step_index = 0;
    game_state.countdown_2 = 0;
    h->substep_index = 0;
    game_state.field_09 = 1;
    game_state.timer_or_bits = 0;
    game_state.field_80 = 0;
    game_state.state_2c_flag = 0xff;
    player_left.field_01 = 0;
    player_right.field_01 = 0;
}
