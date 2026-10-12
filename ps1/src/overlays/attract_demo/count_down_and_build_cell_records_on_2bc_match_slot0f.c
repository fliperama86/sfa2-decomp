/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void build_cell_name_hex_win_records_index_0_slot0f(Object *obj);
void build_cell_name_hex_win_records_index_1_slot0f(Object *obj);
extern void (*data_800f01bc_slot0f[])(Object *);

void count_down_and_build_cell_records_on_2bc_match_slot0f(Object *obj) {
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
                build_cell_name_hex_win_records_index_1_slot0f(obj);
                return;
            }
        } else {
            obj->state_1 = obj->state_1 + 1;
            game_state.field_2bc = 4;
        }
        build_cell_name_hex_win_records_index_0_slot0f(obj);
    }
}

void count_down_and_update_when_2bc_is_2_slot0f(Object *obj) {
    u8 *p = &game_state.field_2bc;
    s16 t;
    if (*p == 2) {
        t = obj->countdown;
        t -= 1;
        obj->countdown = t;
        if (t != 0) {
            if ((t & 3) == 0) {
                build_cell_name_hex_win_records_index_1_slot0f(obj);
                return;
            }
        } else {
            obj->state_1 = 2;
            *p = 0xb;
        }
        build_cell_name_hex_win_records_index_0_slot0f(obj);
    }
}

void call_handler_by_state_1_5_slot0f(Object *obj) {
    data_800f01bc_slot0f[obj->state_1](obj);
}
