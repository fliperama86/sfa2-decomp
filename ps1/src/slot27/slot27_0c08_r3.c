/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80010e44_slot27(void) {
    Object *o;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0xa8;
        o->field_03 = 0;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0x10;
        o->field_03 = 0xa;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0x10;
        o->field_03 = 0xb;
    }
}

void func_80010ed0_slot27(void) {
    Object *o;
    int one;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0x10;
        o->field_03 = 0;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        one = 1;
        o->field_00 = one;
        o->field_02 = 0x10;
        o->field_03 = one;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0x10;
        o->field_03 = 2;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0x10;
        o->field_03 = 8;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0x10;
        o->field_03 = 9;
    }
}
