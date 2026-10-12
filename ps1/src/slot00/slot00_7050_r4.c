/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80079948_slot00[])(Object *);
void add_delta_to_parent_if_counter_zero_slot00(Object *obj, short delta);

void update_follower_copy_parent_position_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    Object *other;
    if (game_state.field_65 == 0 && game_state.field_a8 == 0) {
        other = o->parent;
        if (obj->countdown != ((other->state_0 << 24) | (other->state_1 << 16) | (other->state_2 << 8))) {
            o->state_0 = o->state_0 + 1;
            return;
        }
        o->pos_x = other->pos_x;
        o->pos_y = other->pos_y;
        o->facing_side = other->facing_side;
        data_80079948_slot00[o->state_1](o);
    }
    clear_or_set_flag_by_pos_x_limit(o);
}

void next_state_if_parent_step_flag_clear_slot00(Object *obj) {
    if ((obj->parent->step_flags & 0x80) == 0) {
        obj->state_0 = obj->state_0 + 1;
    }
}

void advance_state_0_add_parent_delta_5_slot00(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
    add_delta_to_parent_if_counter_zero_slot00(obj, 5);
}
