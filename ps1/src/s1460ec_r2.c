/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void update_attached_sequence_unless_pause_flag_b(Object *object) {
    if (game_state.config->pause_flag_b != 0) {
        clear_or_set_flag_by_pos_x_limit(object);
        return;
    }
    if ((s16)object->step_flags < 0) {
        object->state_0 = 2;
    }
    follow_parent_position_with_frame_offset(object);
    advance_sequence_checked(object);
    clear_or_set_flag_by_pos_x_limit(object);
}

/* The parameter is passed on to reset_and_push_object_to_pool_table, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table fn_table_8017c9e0 holds this function and is declared with this parameter. */
void push_object_to_pool_table_wrapper_5(Object *p) {
    reset_and_push_object_to_pool_table((Slab172 *)p);
}

void follow_parent_position_with_frame_offset(Object *object) {
    if (object->table_index & 0x80) {
        Object *src = object->parent;
        Pair *pair;
        int x, y;
        object->pos_x = src->pos_x;
        object->pos_y = src->pos_y;
        if (src->side == 0) {
            pair = pairs_left;
        } else {
            pair = pairs_right;
        }
        pair += src->frame->offset_index & 0xf;
        x = pair->first;
        y = pair->second;
        object->facing_side = src->facing_side;
        if (object->facing_side != 0) {
            x = -x;
        }
        object->pos_x = x + object->pos_x;
        object->pos_y = object->pos_y - y;
    }
}

void set_image_row_from_team_id(Object *object) {
    object->field_0c = 0xff;
    object->image_row = 2;
    if (object->team_id != 0) {
        object->field_0c = 0;
        object->image_row = 0;
    }
}
