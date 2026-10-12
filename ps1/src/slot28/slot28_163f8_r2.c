/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80050c60_slot28[])(Object *);
void func_8002725c_slot28(Object *obj, int arg);

void func_800267b4_slot28(Object *obj) {
    data_80050c60_slot28[data_8018f5a0->pass_index](obj);
    update_blocks();
    update_active_slab172_entries();
}

void func_8002680c_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->pass_index = h->pass_index + 1;
    func_8002725c_slot28(obj, 0);
}
