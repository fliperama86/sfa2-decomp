/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u32 data_80028a40_slot12[];
extern Slot12Prim data_8002c528_slot12[];
extern HudSlot data_8002c4c8_slot12[4];
extern Slot12Prim data_8002c3c8_slot12[];

void link_slot12_prims_to_ordering_table_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    int i;
    int idx;

    if (obj->entry_count >= 0) {
        i = 0;
        idx = draw_buffer_index;
        do {
            ((PrimTag *)&((Slot12Prim (*)[2])data_8002c528_slot12)[i][idx])->addr = ((PrimTag *)current_ordering_table)[data_80028a40_slot12[obj->ot_index]].addr;
            ((PrimTag *)current_ordering_table)[data_80028a40_slot12[obj->ot_index]].addr = (u32)&((Slot12Prim (*)[2])data_8002c528_slot12)[i][idx];
            i++;
        } while (i <= obj->entry_count);
    }
    for (i = 0; i < 4; i++) {
        add_item_to_table(&data_8002c4c8_slot12[i]);
        ((PrimTag *)((u8 *)data_8002c3c8_slot12 + (draw_buffer_index << 5) + (i << 6)))->addr = ((PrimTag *)current_ordering_table)[data_80028a40_slot12[obj->ot_index]].addr;
        ((PrimTag *)current_ordering_table)[data_80028a40_slot12[obj->ot_index]].addr = (u32)((u8 *)data_8002c3c8_slot12 + (i << 6) + (draw_buffer_index << 5));
    }
}
