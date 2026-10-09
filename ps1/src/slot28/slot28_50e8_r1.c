/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051890_slot28;
extern ObjectRef data_80051894_slot28;
extern ObjectRef data_80051898_slot28;
extern ObjectRef data_8005189c_slot28;

void func_800150e8_slot28(Object *obj) {
    func_8011f240((Slab172 *)data_80051890_slot28.p);
    func_8011f240((Slab172 *)data_80051894_slot28.p);
    func_8011f240((Slab172 *)data_80051898_slot28.p);
    func_8011f240((Slab172 *)data_8005189c_slot28.p);
}

void func_80015140_slot28(Object *obj) {
    HudState *h;
    int t;
    if (data_80190949 != 0 && obj->field_f0 == 0) {
        h = data_8018f5a0;
        t = h->field_60 - 1;
        h->field_60 = t;
        if ((s16)t == 0) {
            h->field_52 = h->field_52 + 1;
            func_801282d4();
        }
    }
}
