/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Prim data_800f77b4_slot0f[];
extern Slot12Prim data_800f78f4_slot0f[];
extern Slot12Prim data_800f7a34_slot0f[];
extern void (*data_800eff4c_slot0f[])(Object *);
void fill_sprite_from_sequence_and_link_slot0f(Object *obj, Slot12Sprite *cell);

void draw_variant_sprite_until_signal_slot0f(Object *obj) {
    Slot12Prim *t;

    switch (obj->variant_index) {
    case 0:
        t = data_800f77b4_slot0f;
        break;
    case 1:
        t = data_800f78f4_slot0f;
        break;
    case 2:
        t = data_800f7a34_slot0f;
        break;
    default:
        goto skip;
    }
    fill_sprite_from_sequence_and_link_slot0f(obj, (Slot12Sprite *)((Slot12Prim *)(obj->table_index * 64 + (data_801a27d0 * 32 + (u32)t))));
skip:
    if (game_state.field_2bc == 10) {
        obj->state_0++;
        obj->state_1 = 0;
    }
}

void release_object_record_2_slot0f(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_3_slot0f(Object *obj) {
    data_80190468.p = (Object *)&game_state;
    data_800eff4c_slot0f[obj->state_0](obj);
}
