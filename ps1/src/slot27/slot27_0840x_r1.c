/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u8 data_8002f158_slot27;
void func_801257f4(Select *sel);

/* The mask of t after the load of field_b1 changes nothing in the value. Written without it, this function differs from the original in 11 instruction slots. */
void func_80010840_slot27(void) {
    HudState *h;
    u16 *p;
    int n;
    int t;
    int m;
    data_8002f158_slot27 = 0;
    m = game_state.mode | game_state.field_07;
    if (data_8018f5a0->field_52 == m) {
        data_8018f5a0->field_4e++;
        game_state.field_09 = 1;
        func_801257f4((Select *)&game_state);
        if (game_state.field_b0 != 0) {
            t = game_state.field_b1;
            t &= 0xff;
            if ((t & 0x80) == 0) {
                if (game_state.field_b2 != 0) {
                    if (game_state.field_b2 == 10) {
                        t = 0x13;
                    }
                    if (game_state.field_b2 == 11) {
                        t = 0x12;
                    }
                }
                game_state.field_40 = t;
            }
        }
        h = data_8018f5a0;
        p = &game_state.field_c6;
        *p = 0;
        n = h->field_4e;
        h->field_50 = 0;
        h->field_52 = 0;
        h->field_54 = 0;
        h->field_4e = n + 1;
        if ((game_state.mode | game_state.field_07) == 3) {
            h->field_4e = n + 2;
            *p = 0x40;
        }
    }
}
