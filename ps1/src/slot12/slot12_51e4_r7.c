/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_slot12_cell_record_with_sequence_slot12(Object *obj, int a);
void build_name_text_record_slot12(Object *obj, int a);
void build_hex_value_text_record_slot12(Object *obj, int a);
void build_win_count_text_record_slot12_slot12(Object *obj, int a);

void func_80015824_slot12(Object *obj) {
    if (game_state.field_2bc == 10) {
        obj->state_1 = 0;
        obj->state_0 += 1;
    }
}

void func_80015850_slot12(Object *obj) {
    init_slot12_cell_record_with_sequence_slot12(obj, 0);
    build_name_text_record_slot12(obj, 0);
    build_hex_value_text_record_slot12(obj, 0);
    if (game_state.mode_flag_2 != 0) {
        build_win_count_text_record_slot12_slot12(obj, 0);
    }
}

void func_800158b0_slot12(Object *obj) {
    init_slot12_cell_record_with_sequence_slot12(obj, 1);
    build_name_text_record_slot12(obj, 1);
    build_hex_value_text_record_slot12(obj, 1);
    if (game_state.mode_flag_2 != 0) {
        build_win_count_text_record_slot12_slot12(obj, 1);
    }
}
