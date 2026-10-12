/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8003c294_slot28[])(Object *);
void func_8001d548_slot28(Object *obj, int arg);
extern Object *data_80051a44_slot28[];

void func_8001cd04_slot28(Object *obj) {
    data_8003c294_slot28[data_8018f5a0->pass_index](obj);
    update_blocks();
    update_active_slab172_entries();
}

void func_8001cd5c_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->pass_index = h->pass_index + 1;
    func_8001d548_slot28(obj, 0);
}

void func_8001cd90_slot28(Object *obj) {
    if (*(s16 *)&data_80051a44_slot28[3]->step_flags < 0) {
        HudState *h = data_8018f5a0;
        h->pass_index = h->pass_index + 1;
        start_fade_mode_1801_and_wait();
    }
}
