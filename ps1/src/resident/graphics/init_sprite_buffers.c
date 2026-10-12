/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void init_sprite_buffers(void) {
    int i;
    Cell20 *a;
    Cell20 *b;
    Prim *p;
    Prim *q;
    for (i = 0; i < 256; i++) {
        a = &data_8018a4bc[0][i];
        b = &data_8018a4bc[1][i];
        func_8015c100(a);
        a->color_r = 0x80;
        a->color_g = 0x80;
        a->color_b = 0x80;
        func_8015c100(b);
        b->color_r = 0x80;
        b->color_g = 0x80;
        b->color_b = 0x80;
    }
    for (i = 0; i < 0x30; i++) {
        p = &data_8018ccbc[0][i];
        q = &data_8018ccbc[1][i];
        func_80158a2c(p, 1, 1, func_8015bd0c(0, 0, 0x3c0, 0x100), 0);
        func_80158a2c(q, 1, 1, func_8015bd0c(0, 0, 0x3c0, 0x100), 0);
    }
    data_8018d204 = 0;
}
