/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80028038_slot28(Object *obj);
void func_800281ec_slot28(Object *obj);

void func_80027d9c_slot28(Object *obj) {
    HudState *h;
    func_8011ea68((u8 *)0x800e0000);
    h = data_8018f5a0;
    h->field_60 = 0x12c;
    h->field_50 = h->field_50 + 1;
    func_80028038_slot28(obj);
    func_8012818c();
}

void func_80027df4_slot28(Object *obj) {
    HudState *h;
    int t;
    func_800281ec_slot28(obj);
    h = data_8018f5a0;
    t = h->field_60 - 1;
    h->field_60 = t;
    if ((s16)t < 0) {
        h->field_50 = 0;
        h->field_52 = 0;
        h->field_4e = h->field_4e + 1;
    }
}
