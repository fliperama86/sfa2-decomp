/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



/* Exact. Decided by: address locals a and b declared at function scope and reused in all three
   loops (the original keeps the store address in their registers), and the pos_x values written
   as 0xc + m * 0x10 and 0x154 - j * 0x10 instead of stepped locals. */
void init_strips(void) {
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

            init_draw_mode_prim_3(a);
            init_draw_mode_prim_3(b);
            a->color_r = 0x80;
            a->color_g = 0x80;
            a->color_b = 0x80;
            b->color_r = 0x80;
            b->color_g = 0x80;
            b->color_b = 0x80;
            a->pos_y = 0x1a;
            b->pos_y = 0x1a;
            if (game_state.count_index == 0) {
                a->tex_u = 0x80;
                b->tex_u = 0x80;
                a->tex_v = 0;
                b->tex_v = 0;
            }
            a->tex_param = 0x7f07;
            b->tex_param = 0x7f07;
            a->draw_mode_word = 0xe100001f;
            b->draw_mode_word = 0xe100001f;
        }
    }
    for (k = 0; k < 2; k++) {
        for (m = 0; m < 4; m++) {
            a = &strips[k][m];
            a->pos_x = 0xc + m * 0x10;
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            b = &data_80190014[i][j];
            b->pos_x = 0x154 - j * 0x10;
        }
    }
}
