/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079ff8_slot00[];
void add_delta_to_parent_if_counter_zero_slot00(Object *obj, short delta);

void func_80077640_slot00(Object *obj) {
    Object *p = obj->parent;
    obj->state_0++;
    add_delta_to_parent_if_counter_zero_slot00(obj, 5);
    play_side_sound_at(p, p->side, 0x14);
}

void release_object_record_slot00(Object *obj) {
    reset_and_push_block_object_type_8((Slab172 *)obj);
}

void add_delta_to_parent_if_counter_zero_slot00(Object *obj, short delta) {
    Object *p = obj->parent;
    if (p->counter == 0) {
        add_to_meter_clamped(p, delta);
    }
}

void call_handler_by_state_0_3_slot00(Object *obj) {
    data_80079ff8_slot00[obj->state_0](obj);
}
