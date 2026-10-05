/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* The argument is some other struct with a byte at 0x47 and an s16 at 0x66;
   GameState is used as a stand-in with a signed cast. */
void func_80125b80(GameState *a) {
    u8 *p = &player_left.field_165;
    int x = *p | p[0x394];

    if (a->field_47 == 0 && x != 0) {
        if ((x & 0x80) == 0) {
            a->field_66 = 0xff;
        }
        if ((s16)a->field_66 == 0) {
            a->field_66 = 0xff;
            func_80125dc0(0, 4, 0, 0x15);
            func_80125dc0(0, 1, 0, 0x1a);
            func_80125dc0(0, 7, 8, 0);
            if (p[0x394] == 0) {
                func_80125dc0(0, 5, 8, 0xb);
            }
            if (*p == 0) {
                func_80125dc0(0, 5, 8, 0x10);
            }
            func_80125dc0(1, 0x20, 8, 0);
            func_80125dc0(2, 0x20, 8, 0);
            func_80125dc0(3, 0x20, 8, 0);
            func_80125dc0(4, 0x20, 8, 0);
            func_801260ac(0, 1, 0x1b);
            if (game_state.field_40 == 0xf && game_state.field_226 == 0) {
                if (player_left.kind == 0xf) {
                    func_80125dc0(0, 1, 8, (player_left.field_0d + 2) & 0xff);
                } else if (player_right.kind == 0xf) {
                    func_80125dc0(0, 1, 8, (player_right.field_0d + 2) & 0xff);
                }
            }
            func_80137b10();
        }
    } else if ((s16)a->field_66 != 0) {
        a->field_66 = 0;
        func_801260ac(0, 0x20, 0);
        func_801260ac(1, 0x20, 0);
        func_801260ac(2, 0x20, 0);
        func_801260ac(3, 0x20, 0);
        func_801260ac(4, 0x20, 0);
        func_80137b10();
    }
}
