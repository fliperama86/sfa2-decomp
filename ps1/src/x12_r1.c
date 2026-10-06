/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern Strip1c strips2[2][2];
extern Strip1c data_80188e4c[2][2];

void func_80132cf0(void) {
    s32 i;
    s32 j;
    Strip1c *p;
    Strip1c *q;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            Strip1c *a = &strips2[i][j];
            Strip1c *b = &data_80188e4c[i][j];

            func_80136d1c((Tx *)a);
            func_80136d1c((Tx *)b);
            a->field_10 = 0x80;
            a->field_11 = 0x80;
            a->field_12 = 0x80;
            b->field_10 = 0x80;
            b->field_11 = 0x80;
            b->field_12 = 0x80;
            a->field_16 = 0;
            b->field_16 = 0;
            a->field_19 = 0xe0;
            b->field_19 = 0xe0;
            a->field_1a = 0x7f07;
            b->field_1a = 0x7f07;
            a->field_04 = 0xe100001f;
            b->field_04 = 0xe100001f;
        }
    }
    for (i = 0; i < 2; i++) {
        p = &strips2[i][0];
        q = &strips2[i][1];
        p->field_18 = 0;
        q->field_18 = 0x10;
        p->field_14 = 0x78;
        q->field_14 = 0x88;
    }
    for (i = 0; i < 2; i++) {
        p = &data_80188e4c[i][0];
        q = &data_80188e4c[i][1];
        p->field_18 = 0;
        q->field_18 = 0x10;
        p->field_14 = 0xd8;
        q->field_14 = 0xe8;
    }
}
