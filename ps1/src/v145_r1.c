/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Exact. Decided by: address locals a and b declared at function scope and reused in all three
   loops (the original keeps the store address in their registers), and the field_14 values written
   as 0xc + m * 0x10 and 0x154 - j * 0x10 instead of stepped locals. */
void func_80132b30(void) {
    s32 i;
    s32 j;
    s32 x;
    s32 k;
    s32 m;
    Strip1c *a;
    Strip1c *b;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            a = &strips[i][j];
            b = &data_80190014[i][j];

            func_80136d1c(a);
            func_80136d1c(b);
            a->field_10 = 0x80;
            a->field_11 = 0x80;
            a->field_12 = 0x80;
            b->field_10 = 0x80;
            b->field_11 = 0x80;
            b->field_12 = 0x80;
            a->field_16 = 0x1a;
            b->field_16 = 0x1a;
            if (game_state.field_42 == 0) {
                a->field_18 = 0x80;
                b->field_18 = 0x80;
                a->field_19 = 0;
                b->field_19 = 0;
            }
            a->field_1a = 0x7f07;
            b->field_1a = 0x7f07;
            a->field_04 = 0xe100001f;
            b->field_04 = 0xe100001f;
        }
    }
    for (k = 0; k < 2; k++) {
        for (m = 0; m < 4; m++) {
            a = &strips[k][m];
            a->field_14 = 0xc + m * 0x10;
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            b = &data_80190014[i][j];
            b->field_14 = 0x154 - j * 0x10;
        }
    }
}
