/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80028038_slot28(Object *obj);
void func_800281ec_slot28(Object *obj);

void func_80027d9c_slot28(Object *obj) {
    HudState *h;
    load_image_from_stream((u8 *)0x800e0000);
    h = data_8018f5a0;
    h->countdown_timer = 0x12c;
    h->step_index_2 = h->step_index_2 + 1;
    func_80028038_slot28(obj);
    func_8012818c();
}

void func_80027df4_slot28(Object *obj) {
    HudState *h;
    int t;
    func_800281ec_slot28(obj);
    h = data_8018f5a0;
    t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        h->step_index_2 = 0;
        h->pass_index = 0;
        h->substep_index = h->substep_index + 1;
    }
}
