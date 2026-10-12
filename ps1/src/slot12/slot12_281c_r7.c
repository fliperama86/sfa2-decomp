/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void draw_bit_grid_sprites_for_object_slot12(Object *obj);
void sync_variant_index_from_game_object_slot12(Object *obj);

void func_80012eb0_slot12(Object *obj) {
    if ((game_state.side_bits_b >> game_state.object->side) & 1) {
        if ((game_state.status_flags & 0x80) == 0) {
            draw_bit_grid_sprites_for_object_slot12(obj);
            sync_variant_index_from_game_object_slot12(obj);
            obj->y_offset = obj->health - 1;
        }
    } else {
        obj->state_0++;
    }
}

void func_80012f44_slot12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
