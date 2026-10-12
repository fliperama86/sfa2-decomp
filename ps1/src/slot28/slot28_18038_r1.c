/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Poly28 data_80051df4_slot28[];

void func_80028038_slot28(Object *obj) {
    int i;

    Poly28 *p = data_80051df4_slot28;

    for (i = 0; i < 2; p++, i++) {
        func_8015c09c(p);
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->tpage = func_8015bd0c(1, 0, 0x180, 0);
        p->clut = func_8015bdd4(0x180, 0xff);
        p->x0 = 0;
        p->y0 = 0;
        p->x1 = 0x100;
        p->y1 = 0;
        p->x2 = 0;
        p->y2 = 0xf0;
        p->x3 = 0x100;
        p->y3 = 0xf0;
        p->u0 = 0;
        p->v0 = 0;
        p->u1 = 0xff;
        p->v1 = 0;
        p->u2 = 0;
        p->v2 = 0xf0;
        p->u3 = 0xff;
        p->v3 = 0xf0;
    }
    p = &data_80051df4_slot28[2];
    for (i = 0; i < 2; p++, i++) {
        func_8015c09c(p);
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->tpage = func_8015bd0c(1, 0, 0x200, 0);
        p->clut = func_8015bdd4(0x180, 0xff);
        p->x0 = 0x100;
        p->y0 = 0;
        p->x1 = 0x180;
        p->y1 = 0;
        p->x2 = 0x100;
        p->y2 = 0xf0;
        p->x3 = 0x180;
        p->y3 = 0xf0;
        p->u0 = 0;
        p->v0 = 0;
        p->u1 = 0x80;
        p->v1 = 0;
        p->u2 = 0;
        p->v2 = 0xf0;
        p->u3 = 0x80;
        p->v3 = 0xf0;
    }
}
