/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80168f88(short a, short b, short c) {
    Rec172 *e = &table_801ac628[a][b];
    e->field_90 &= ~0x200;
    table_801ac628[a][b].field_90 &= ~4;
    table_801ac628[a][b].field_90 |= 1;
    e->current_volume = e->target_volume;
    e->set_value = c;
    e->field_48 = 0;
    func_80163234(a | (b << 8), e->seq_volume_left, e->seq_volume_right, 0);
}
