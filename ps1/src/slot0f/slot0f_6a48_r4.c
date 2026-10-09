/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Prim data_800f7b94_slot0f[];
extern HudSlot data_800f7c94_slot0f[4];

void func_800e6dfc_slot0f(Object *obj) {
    s16 i;

    for (i = 0; i < 4; i++) {
        func_801519b4(&data_800f7c94_slot0f[i]);
        ((PrimTag *)((u8 *)data_800f7b94_slot0f + (data_801a27d0 << 5) + (i << 6)))->addr = ((PrimTag *)data_801987c8)->addr;
        ((PrimTag *)data_801987c8)->addr = (u32)((u8 *)data_800f7b94_slot0f + (i << 6) + (data_801a27d0 << 5));
    }
    if (game_state.field_2bc == 10) {
        obj->field_04++;
    }
}

void func_800e6f0c_slot0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
