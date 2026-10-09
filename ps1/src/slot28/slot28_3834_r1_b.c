/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_800289f4_slot28[])(Object *);

void func_8001392c_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    data_800289f4_slot28[obj->field_70 & 0x7f](o);
}

void func_80013970_slot28(Object *obj) {
}

void func_80013978_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->field_60 = 1;
    h->field_50 = 0;
    h->field_52 = 0;
    h->field_4e = h->field_4e + 1;
    obj->field_ad = 1;
    func_8011eae4();
}
