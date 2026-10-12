/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800284d8_slot12[])(Object *);
void build_cell_name_hex_win_records_slot12_index_0_slot12(Object *obj);
void build_cell_name_hex_win_records_slot12_index_1_slot12(Object *obj);
void spawn_variant_2_object_if_offset_slot12(Object *obj, int a);

void start_countdown_and_spawn_effects_slot12(Object *obj) {
    u8 t = obj->state_1;
    obj->countdown = 0x1e;
    obj->state_1 = t + 1;
    build_cell_name_hex_win_records_slot12_index_0_slot12(obj);
    spawn_variant_2_object_if_offset_slot12(obj, 0);
    game_state.field_2bc = 1;
}

void count_down_and_build_slot12_cell_records_slot12(Object *obj) {
    s16 t;
    int a;
    int b;
    if (game_state.mode_flag_2 == 0) {
        a = game_state.field_2bc;
        b = 3;
    } else {
        a = game_state.field_2bc;
        b = 5;
    }
    if (a == b) {
        t = obj->countdown;
        t -= 1;
        obj->countdown = t;
        if (t != 0) {
            if ((t & 3) == 0) {
                build_cell_name_hex_win_records_slot12_index_1_slot12(obj);
                return;
            }
        } else {
            obj->state_1 = obj->state_1 + 1;
            game_state.field_2bc = 4;
        }
        build_cell_name_hex_win_records_slot12_index_0_slot12(obj);
    }
}

void call_handler_by_state_1_5_slot12(Object *obj) {
    data_800284d8_slot12[obj->state_1](obj);
}
