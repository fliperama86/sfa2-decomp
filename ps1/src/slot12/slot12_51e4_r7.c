/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_slot12_cell_record_with_sequence_slot12(Object *obj, int a);
void build_name_text_record_slot12(Object *obj, int a);
void build_hex_value_text_record_slot12(Object *obj, int a);
void build_win_count_text_record_slot12_slot12(Object *obj, int a);

void advance_state_0_if_game_field_equals_ten_slot12(Object *obj) {
    if (game_state.field_2bc == 10) {
        obj->state_1 = 0;
        obj->state_0 += 1;
    }
}

void build_cell_name_hex_win_records_slot12_index_0_slot12(Object *obj) {
    init_slot12_cell_record_with_sequence_slot12(obj, 0);
    build_name_text_record_slot12(obj, 0);
    build_hex_value_text_record_slot12(obj, 0);
    if (game_state.mode_flag_2 != 0) {
        build_win_count_text_record_slot12_slot12(obj, 0);
    }
}

void build_cell_name_hex_win_records_slot12_index_1_slot12(Object *obj) {
    init_slot12_cell_record_with_sequence_slot12(obj, 1);
    build_name_text_record_slot12(obj, 1);
    build_hex_value_text_record_slot12(obj, 1);
    if (game_state.mode_flag_2 != 0) {
        build_win_count_text_record_slot12_slot12(obj, 1);
    }
}
