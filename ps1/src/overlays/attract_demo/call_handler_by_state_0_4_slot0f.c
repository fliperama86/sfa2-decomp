/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800eff9c_slot0f[])(Object *);
extern Slot12Prim data_800f7cf4_slot0f[];
extern SequenceStep *data_800eff98_slot0f;
void init_two_prims_with_draw_mode_slot0f(Object *obj, Slot12Prim *c, int x, int y);
void fill_sprite_from_sequence_and_link_slot0f(Object *obj, Slot12Sprite *cell);

void call_handler_by_state_0_4_slot0f(Object *obj) {
    data_800eff9c_slot0f[obj->state_0](obj);
}

void init_object_position_sequence_and_two_prims_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    obj->pos_x = 0xc0;
    obj->base_y = 0x80;
    o->ot_depth = 5;
    o->field_01 = 0;
    o->state_0++;
    set_sequence_from_table_5(o, 0, &data_800eff98_slot0f);
    init_two_prims_with_draw_mode_slot0f(o, data_800f7cf4_slot0f, 0x100, 0xc0);
}

void draw_background_sprite_advance_state_slot0f(Object *obj) {
    fill_sprite_from_sequence_and_link_slot0f(obj, (Slot12Sprite *)(&data_800f7cf4_slot0f[data_801a27d0]));
    if (game_state.field_2bc == 10) {
        obj->state_0++;
    }
}

void release_object_record_4_slot0f(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
