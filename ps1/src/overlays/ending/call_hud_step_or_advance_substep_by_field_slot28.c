/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void reset_hud_and_game_state_fields_slot28(void);
void reset_hud_state_and_init_pools_slot28(void);
void call_handler_by_hud_step_index_2_19_slot28(Object *obj);
void call_handler_by_hud_step_index_2_20_slot28(Object *obj);

void call_hud_step_or_advance_substep_by_field_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    if (obj->field_1c == 0) {
        call_handler_by_hud_step_index_2_19_slot28((Object *)&game_state);
    } else {
        HudState *h = screen_state;
        h->substep_index = h->substep_index + 1;
    }
}

void call_hud_step_or_advance_substep_by_byte_slot28(Object *obj) {
    if ((s8)data_8016e685 >= 2) {
        call_handler_by_hud_step_index_2_20_slot28((Object *)&game_state);
    } else {
        HudState *h = screen_state;
        h->substep_index = h->substep_index + 1;
    }
}

void reset_players_and_hud_indices_when_idle_slot28(Object *o) {
    if (((Slot28Obj *)o)->field_f0 == 0) {
        HudState *h;
        ((Slot28Obj *)o)->field_19 = 0;
        h = screen_state;
        player_left.field_a5 = 0;
        player_right.field_a5 = 0;
        player_left.field_01 = 0;
        player_right.field_01 = 0;
        h->step_index_2 = 0;
        h->pass_index = 0;
        h->field_54 = 0;
        h->field_56 = 0;
        ((Slot28Obj *)o)->field_2c = 0;
        ((Slot28Obj *)o)->state_flag = 1;
        if (((u8 *)&o->field_90)[1] != 0) {
            ((Slot28Obj *)o)->state_flag = 0;
        }
        ((Slot28Obj *)o)->state_flag = ((Slot28Obj *)o)->state_flag + 0x80;
        reset_hud_state_and_init_pools_slot28();
    }
}

void reset_hud_state_and_init_pools_slot28(void) {
    reset_hud_and_game_state_fields_slot28();
    game_state.field_44 = 0xff;
    clear_arrays_and_init_pools_and_sprite_tables();
}
