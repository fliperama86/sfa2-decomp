/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8016a0cc(short a, short b) {
    Rec172 *e = &table_801ac628[a][b];
    func_801634c4((b << 8) | a);
    e->field_2b = 0;
    table_801ac628[a][b].field_90 &= ~2;
}
