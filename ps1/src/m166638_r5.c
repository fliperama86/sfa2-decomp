/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80168e18(short a, short b, unsigned char c, short d) {
    Rec172 *e = &table_801ac628[a][b];
    e->field_90 &= ~0x200;
    table_801ac628[a][b].field_90 &= ~4;
    e->field_46 = d;
    if (c == 1) {
        table_801ac628[a][b].field_90 |= 1;
        e->field_48 = 0;
        e->field_2b = 1;
        func_80163234(a | (b << 8), e->field_74, e->field_76, 0);
    } else if (c == 0) {
        table_801ac628[a][b].field_90 |= 2;
    }
}
