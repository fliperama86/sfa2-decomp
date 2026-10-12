/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_801b8ca8_slot04_sel[])(Object *, Slot04SelRec9d78 *);
extern Slot04SelRec9d78 data_801b9d78_slot04_sel[];

void finish_selection_and_advance_substep_slot04_sel(Object *unused_obj, Slot04SelRec9d78 *unused_rec) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    Object *p = &player_left;
    Object *q = p + 1;
    Slot04SelRec9d78 *r = data_801b9d78_slot04_sel;
    Slot04SelRec9d78 *s = r + 1;
    game_state.field_6d = 0;
    data_801b8ca8_slot04_sel[1](p, r);
    data_801b8ca8_slot04_sel[1](q, s);
    if (game_state.pause_flag != 0) {
        data_8018f5a0->substep_index = data_8018f5a0->substep_index + 1;
        data_8018f5a0->step_index_2 = 0;
        game_state.pause_flag = 0;
    } else {
        data_8018f5a0->substep_index = data_8018f5a0->substep_index + 2;
        data_8018f5a0->step_index_2 = 0;
        game_state.field_80 = 0;
        game_state.b0_flag = 0;
        game_state.selection_value = 0xff;
        game_state.b2_count = 0;
    }
    data_801ae028 = 0;
}

void init_selection_record_initial_value_3_slot04_sel(Object *obj, Slot04SelRec9d78 *rec) {
    rec->field_00 = 0;
    rec->selection_index = 0;
    rec->field_04 = 0;
    rec->initial_value = 3;
    rec->field_07 = 0;
    rec->field_08 = 0;
    rec->field_09 = 0;
    rec->field_0a = 0;
    rec->field_0b = 0;
    rec->field_0c = 0;
    rec->field_0d = 0;
    rec->field_0e = 0;
    rec->set_flag_a = 0;
    rec->set_flag_b = 0;
    rec->sequence_index = 0;
    rec->set_flag_c = 0;
    rec->set_flag_d = 0;
    rec->field_14 = 0;
    rec->hold_count_b = 0;
}
