/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void spawn_three_pool_objects_slot27(void) {
    Object *o;
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0xa8;
        o->table_index = 0;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x10;
        o->table_index = 0xa;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x10;
        o->table_index = 0xb;
    }
}

void spawn_five_pool_objects_10_indexed_slot27(void) {
    Object *o;
    int one;
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x10;
        o->table_index = 0;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        one = 1;
        o->active_count = one;
        o->field_02 = 0x10;
        o->table_index = one;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x10;
        o->table_index = 2;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x10;
        o->table_index = 8;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x10;
        o->table_index = 9;
    }
}
