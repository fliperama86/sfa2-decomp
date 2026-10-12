/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80010e44_slot27(void) {
    Object *o;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0xa8;
        o->table_index = 0;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x10;
        o->table_index = 0xa;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x10;
        o->table_index = 0xb;
    }
}

void func_80010ed0_slot27(void) {
    Object *o;
    int one;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x10;
        o->table_index = 0;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        one = 1;
        o->active_count = one;
        o->field_02 = 0x10;
        o->table_index = one;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x10;
        o->table_index = 2;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x10;
        o->table_index = 8;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x10;
        o->table_index = 9;
    }
}
