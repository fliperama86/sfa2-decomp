/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot12Prim data_800f7b94_slot0f[];
extern HudSlot data_800f7c94_slot0f[4];

void link_hud_slots_to_ordering_table_slot0f(Object *obj) {
    s16 i;

    for (i = 0; i < 4; i++) {
        add_item_to_table(&data_800f7c94_slot0f[i]);
        ((PrimTag *)((u8 *)data_800f7b94_slot0f + (draw_buffer_index << 5) + (i << 6)))->addr = ((PrimTag *)current_ordering_table)->addr;
        ((PrimTag *)current_ordering_table)->addr = (u32)((u8 *)data_800f7b94_slot0f + (i << 6) + (draw_buffer_index << 5));
    }
    if (game_state.field_2bc == 10) {
        obj->state_0++;
    }
}

void release_object_record_3_slot0f(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
