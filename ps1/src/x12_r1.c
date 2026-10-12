/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void init_strips2(void) {
    s32 i;
    s32 j;
    Strip1c *p;
    Strip1c *q;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            Strip1c *a = &strips2[i][j];
            Strip1c *b = &data_80188e4c[i][j];

            init_draw_mode_prim_3((Tx *)a);
            init_draw_mode_prim_3((Tx *)b);
            a->color_r = 0x80;
            a->color_g = 0x80;
            a->color_b = 0x80;
            b->color_r = 0x80;
            b->color_g = 0x80;
            b->color_b = 0x80;
            a->pos_y = 0;
            b->pos_y = 0;
            a->tex_v = 0xe0;
            b->tex_v = 0xe0;
            a->tex_param = 0x7f07;
            b->tex_param = 0x7f07;
            a->draw_mode_word = 0xe100001f;
            b->draw_mode_word = 0xe100001f;
        }
    }
    for (i = 0; i < 2; i++) {
        p = &strips2[i][0];
        q = &strips2[i][1];
        p->tex_u = 0;
        q->tex_u = 0x10;
        p->pos_x = 0x78;
        q->pos_x = 0x88;
    }
    for (i = 0; i < 2; i++) {
        p = &data_80188e4c[i][0];
        q = &data_80188e4c[i][1];
        p->tex_u = 0;
        q->tex_u = 0x10;
        p->pos_x = 0xd8;
        q->pos_x = 0xe8;
    }
}
