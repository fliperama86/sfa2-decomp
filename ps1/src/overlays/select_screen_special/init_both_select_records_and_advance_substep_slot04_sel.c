/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_801b6d60_slot04_sel[])(Object *, Slot04SelRec *);
extern Slot04SelRec data_801b9cf0_slot04_sel[];

void init_both_select_records_and_advance_substep_slot04_sel(Object *unused_obj, Slot04SelRec *unused_rec) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    Object *l = &player_left;
    Object *r = l + 1;
    Slot04SelRec *a = data_801b9cf0_slot04_sel;
    Slot04SelRec *b = a + 1;

    game_state.field_6d = 0;
    data_801b6d60_slot04_sel[1](l, a);
    data_801b6d60_slot04_sel[2](r, b);
    if (game_state.pause_flag != 0) {
        screen_state->substep_index += 1;
        screen_state->step_index_2 = 0;
        game_state.pause_flag = 0;
    } else {
        screen_state->substep_index += 2;
        screen_state->step_index_2 = 0;
        game_state.field_80 = 0;
        game_state.b0_flag = 0;
        game_state.selection_value = 0xff;
        game_state.b2_count = 0;
        game_state.field_09 = 0;
    }
}

void init_select_record_from_object_slot04_sel(Object *obj, Slot04SelRec *rec) {
    rec->state = 0;
    rec->kind = obj->kind;
    rec->field_04 = 0;
    rec->cursor_index = data_8016e694[obj->side];
    rec->confirm_mask = 0;
    rec->second_cursor_index = data_8016e696;
    rec->field_0b = 0;
    rec->slot_a_flag = 0;
    rec->slot_b_flag = 0;
    rec->sequence_index = 0;
    rec->slot_c_flag = 0;
    rec->slot_d_flag = 0;
    rec->field_17 = 0;
    rec->field_18 = 0;
}
