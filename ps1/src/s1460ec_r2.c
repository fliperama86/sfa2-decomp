/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014630c(Object *object) {
    if (game_state.config->pause_flag_b != 0) {
        func_8011ffdc(object);
        return;
    }
    if ((s16)object->step_flags < 0) {
        object->state_0 = 2;
    }
    func_80146394(object);
    advance_sequence_checked(object);
    func_8011ffdc(object);
}

/* The parameter is passed on to func_8011f240, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table fn_table_8017c9e0 holds this function and is declared with this parameter. */
void func_80146374(Object *p) {
    func_8011f240((Slab172 *)p);
}

void func_80146394(Object *object) {
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

void func_80146450(Object *object) {
    object->field_0c = 0xff;
    object->image_row = 2;
    if (object->team_id != 0) {
        object->field_0c = 0;
        object->image_row = 0;
    }
}
