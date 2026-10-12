/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot12Prim data_8002c3c8_slot12[];
extern HudSlot data_8002c4c8_slot12[4];
extern Slot12Prim data_8002c528_slot12[];
extern u16 data_80028a58_slot12[];
extern u16 data_80028a60_slot12[];

void set_slot12_row_prim_positions_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    Slot12Prim *p;
    int i = 0;
    if (obj->entry_count >= 0) {
        p = &data_8002c528_slot12[data_801a27d0];
        do {
            p[i * 2].x = obj->base_x + i * 16 + 16;
            p[i * 2].y = obj->base_y;
            i++;
        } while (obj->entry_count >= i);
    }
}

void set_slot12_end_prim_and_hud_positions_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    int i = 0;
    int t = obj->entry_count << 4;
    Slot12Prim *p = &data_8002c3c8_slot12[data_801a27d0];
    HudSlot *q;
    do {
        p[i * 2].x = obj->base_x + data_80028a58_slot12[i] + t + 16;
        p[i * 2].y = obj->base_y;
        i++;
    } while (i < 4);
    i = 0;
    q = data_8002c4c8_slot12;
    do {
        q[i].pos_x = obj->base_x + data_80028a60_slot12[i] + t + 16;
        q[i].pos_y = obj->base_y + 8;
        i++;
    } while (i < 4);
}

void release_object_to_pool_table_11_slot12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
