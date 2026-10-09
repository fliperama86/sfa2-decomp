/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80055ea4_slot01;
extern int data_80055f3c_slot01;
extern char data_800100a0_slot01[];
char *strcat(char *dst, const char *src);

void func_80010d0c_slot01(int value, char *dst) {
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

void func_80010dcc_slot01(GameState *g) {
    HudState *h;
    func_8014f4d4(6, 2);
    game_state.field_40 = *(u16 *)&data_80055f3c_slot01;
    h = data_8018f5a0;
    data_80190568 = 0;
    data_80055ea4_slot01 = 0;
    h->field_4c = 0;
    game_state.field_c6 = 0;
    h->field_4e = 0;
    game_state.field_09 = 1;
    game_state.field_c8 = 0;
    game_state.field_80 = 0;
    game_state.field_2c = 0xff;
    player_left.field_01 = 0;
    player_right.field_01 = 0;
}
