/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Poly28 data_80051df4_slot28[];

void link_two_quads_into_ordering_table_slot28(Object *obj) {
    ((PrimTag *)&data_80051df4_slot28[draw_buffer_index])->addr = ((PrimTag *)current_ordering_table)[26].addr;
    ((PrimTag *)current_ordering_table)[26].addr = (u32)&data_80051df4_slot28[draw_buffer_index];
    ((PrimTag *)&data_80051df4_slot28[draw_buffer_index + 2])->addr = ((PrimTag *)current_ordering_table)[26].addr;
    ((PrimTag *)current_ordering_table)[26].addr = (u32)&data_80051df4_slot28[draw_buffer_index + 2];
}
