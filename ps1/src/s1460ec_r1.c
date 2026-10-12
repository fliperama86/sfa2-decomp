/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void get_three_pool_objects_sorted(u32 *p) {
    p[0] = (u32)pop_block_from_pool();
    p[1] = (u32)pop_block_from_pool();
    p[2] = (u32)pop_block_from_pool();
    if (p[0] > p[1]) {
        u32 t = p[0];
        p[0] = p[1];
        p[1] = t;
    }
    if (p[0] > p[2]) {
        u32 t = p[0];
        p[0] = p[2];
        p[2] = t;
    }
    if (p[1] > p[2]) {
        u32 t = p[1];
        p[1] = p[2];
        p[2] = t;
    }
}

void call_state_0_handler_from_c9e0_table(Block172 *block) {
    Object *object = (Object *)block;
    fn_table_8017c9e0[object->state_0](object);
}
