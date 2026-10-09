/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002d574_slot12;

void func_8001047c_slot12(void) {
    Object *o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0x7b;
        o->field_03 = 0;
        o->field_7a = 0;
        o->field_7c = 0x1e1;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0x7b;
        o->field_03 = 0x80;
        data_8002d574_slot12 = o;
    }
    data_8018f5a0->field_4e++;
}

void func_80010510_slot12(void) {
    data_8018f5a0->field_4e++;
}

void func_80010530_slot12(void) {
    data_8018f5a0->field_4e++;
}
