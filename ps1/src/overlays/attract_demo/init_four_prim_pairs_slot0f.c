/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot12Prim data_800f7b94_slot0f[];
void init_two_prims_with_draw_mode_slot0f(Object *obj, Slot12Prim *p, int a, int b);
void fill_two_prims_from_list_slot0f(Object *obj, Slot12Prim *p, int n);

void init_four_prim_pairs_slot0f(Object *obj) {
    int i;

    for (i = 0; i < 4; i++) {
        init_two_prims_with_draw_mode_slot0f(obj, &data_800f7b94_slot0f[i * 2], 0x10, 0x10);
        fill_two_prims_from_list_slot0f(obj, &data_800f7b94_slot0f[i * 2], (u16)i);
    }
}
