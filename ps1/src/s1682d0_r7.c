/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80169abc(short a, short b) {
    table_801ac628[a][b].field_2b = 1;
    table_801ac628[a][b].field_90 &= ~8;
}

void func_80169b24(short a, short b) {
    func_80166274(a, b);
}
