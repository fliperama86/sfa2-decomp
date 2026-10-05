/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801683e8(short a, short b) {
    Rec172 *e = &table_801ac628[a][b];
    e->field_46 = 1;
    e->field_48 = 0;
    table_801ac628[a][b].field_90 &= ~0x100;
    table_801ac628[a][b].field_90 &= ~8;
    table_801ac628[a][b].field_90 &= ~2;
    table_801ac628[a][b].field_90 &= ~4;
    table_801ac628[a][b].field_90 &= ~0x200;
    e->field_2b = 1;
    table_801ac628[a][b].field_90 |= 1;
}
