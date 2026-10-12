/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];

void wait_parent_state_and_set_timer_bit_slot27(Object *obj) {
    if (*(u16 *)&obj->parent->state_0 == 0x101) {
        obj->state_0++;
    }
    game_state.timer_or_bits |= 2;
}

void noop_slot00_2_slot27(Object *obj) {
}

void release_object_2_slot27(Object *object) {
    reset_and_push_object_to_pool_table((Slab172 *)object);
}

void set_position_from_linked_object_offset_slot27(Object *obj) {
    Object *src = (Object *)obj->linked_object;
    int k = obj->table_index - 1;
    obj->pos_x = src->pos_x + (k << 8);
    obj->pos_y = src->pos_y;
    func_8011ffdc(obj);
}
