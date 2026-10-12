/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_80028a88_slot12[])(Object *);
extern Slot12Prim data_8002d528_slot12[];
extern SequenceStep *data_80028a84_slot12;
void init_two_prims_with_draw_mode_slot12(Object *obj, Slot12Prim *cells, int a, int b);
void build_sprite_from_sequence_data_and_link_slot12(Object *obj, Slot12Sprite *cell);

void call_handler_by_state_0_8_slot12(Object *obj) {
    data_80028a88_slot12[obj->state_0](obj);
}

void init_slot12_object_position_and_sequence_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    obj->base_x = 0xc0;
    obj->base_y = 0x80;
    obj->ot_index = 5;
    obj->field_01 = 0;
    obj->step_count++;
    set_sequence_from_table_5(o, 0, &data_80028a84_slot12);
    init_two_prims_with_draw_mode_slot12(o, data_8002d528_slot12, 0x100, 0xc0);
}

void draw_background_sprite_advance_state_slot12(Object *obj) {
    build_sprite_from_sequence_data_and_link_slot12(obj, (Slot12Sprite *)(&data_8002d528_slot12[data_801a27d0]));
    if (game_state.field_2bc == 10) {
        obj->state_0++;
    }
}

void release_object_to_pool_table_12_slot12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
