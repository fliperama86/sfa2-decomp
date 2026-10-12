/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_slot12_row_prim_positions_slot12(Object *o);
void set_slot12_end_prim_and_hud_positions_slot12(Object *o);

void increment_state_1_if_game_field_equals_five_slot12(Object *obj) {
    if (game_state.field_2bc == 5) {
        obj->state_1++;
    }
}

void move_base_x_left_and_update_prim_positions_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    obj->base_x--;
    obj->field_4c--;
    if (obj->field_4c == 0) {
        obj->tick_count++;
    }
    set_slot12_row_prim_positions_slot12(o);
    set_slot12_end_prim_and_hud_positions_slot12(o);
}

void advance_state_1_and_set_stage_3_slot12(Object *obj) {
    obj->state_1++;
    game_state.field_2bc = 3;
}

void update_positions_advance_state_slot12(Object *obj) {
    set_slot12_end_prim_and_hud_positions_slot12(obj);
    set_slot12_row_prim_positions_slot12(obj);
    if (game_state.field_2bc == 10) {
        obj->state_0++;
    }
}
