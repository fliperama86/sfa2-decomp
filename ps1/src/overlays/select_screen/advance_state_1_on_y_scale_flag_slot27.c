/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void advance_palette_animation_and_upload_slot27(Object *obj);

void advance_state_1_on_y_scale_flag_slot27(Object *obj) {
    if (obj->y_scale & 0x8000) {
        obj->state_2 = 0;
        obj->state_3 = 0;
        obj->state_1 = obj->state_1 + 1;
    }
    advance_palette_animation_and_upload_slot27(obj);
}

void update_selection_from_pressed_buttons_slot27(void) {
    int *sel = (int *)&game_state.field_354;

    *sel = game_state.selection;
    if (game_state.busy_flag == 0) {
        int *pad = scratch_word_b;

        *pad = ~game_state.other_object->prev_input_bits;
        *pad = game_state.other_object->input_bits & *pad;
        if (*pad & 0x8000) {
            *sel = 0;
        }
        if (*pad & 0x2000) {
            *sel = 1;
        }
    }
}

void set_side_bit_in_pass_index_slot27(Object *obj) {
    int side;
    u16 *h;

    obj->state_0 = obj->state_0 + 1;
    side = game_state.other_object->side;
    *(int *)&game_state.field_354 = side;
    h = &screen_state->pass_index;
    *h |= 1 << side;
}

void increment_state_0_slot27(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
}

void release_object_to_pool_table_slot27(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
