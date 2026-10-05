/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80168f18(short a, unsigned char b, short c) {
    func_80168e18(a, 0, b, c);
}

void func_80168f50(short a, short b, unsigned char c, short d) {
    func_80168e18(a, b, c, d);
}

void func_80168f88(short a, short b, short c) {
    Rec172 *e = &table_801ac628[a][b];
    e->field_90 &= ~0x200;
    table_801ac628[a][b].field_90 &= ~4;
    table_801ac628[a][b].field_90 |= 1;
    e->field_04 = e->field_08;
    e->field_46 = c;
    e->field_48 = 0;
    func_80163234(a | (b << 8), e->field_74, e->field_76, 0);
}
