/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot04SelRec9d38 data_801b9d38_slot04_sel[];
extern void (*data_801b7c44_slot04_sel[])(Object *, Slot04SelRec9d38 *);

void init_both_records_and_advance_substep_slot04_sel(Object *unused_obj, Slot04SelRec9d38 *unused_rec) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    Object *left = &player_left;
    Object *right = left + 1;
    Slot04SelRec9d38 *rec = data_801b9d38_slot04_sel;
    Slot04SelRec9d38 *rec2 = rec + 1;

    game_state.field_6d = 0;
    data_801b7c44_slot04_sel[1](left, rec);
    data_801b7c44_slot04_sel[2](right, rec2);
    game_state.side_mask = 3;
    if (game_state.pause_flag != 0) {
        data_8018f5a0->substep_index++;
        data_8018f5a0->step_index_2 = 0;
        game_state.pause_flag = 0;
    } else {
        data_8018f5a0->substep_index += 2;
        data_8018f5a0->step_index_2 = 0;
        game_state.field_80 = 0;
        game_state.b0_flag = 0;
        game_state.selection_value = 0xff;
        game_state.b2_count = 0;
    }
}

void init_selection_record_from_object_slot04_sel(Object *obj, Slot04SelRec9d38 *rec) {
    rec->count = 0;
    rec->kind_index = obj->kind;
    rec->field_04 = 0;
    rec->saved_index_a = data_8016e698[obj->side];
    rec->field_07 = 0;
    rec->saved_index_b = data_8016e698[obj->side + 2];
    rec->field_09 = 0;
    rec->saved_index_c = data_8016e698[4];
    rec->field_0b = 0;
    rec->table_index = data_8016e698[5];
    rec->active_flag = 0;
    rec->field_0e = 0;
    rec->field_0f = 0;
    rec->step_count = 0;
    rec->field_11 = 0;
    rec->field_12 = 0;
    rec->count_2 = 0;
    rec->field_14 = 0;
}
