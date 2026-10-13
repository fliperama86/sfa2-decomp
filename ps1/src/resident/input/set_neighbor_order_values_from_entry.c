/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void set_neighbor_order_values_from_entry(Object *object, u8 index, u8 unused) {
    Quad order = data_8016d9a4;
    int step = object->slots[index].entry_index;

    game_state.step_value = step;
    data_80188f4c = (step + 1) & 3;
    data_80188f50 = (step - 1) & 3;
    data_80188f4c = order.v[data_80188f4c];
    data_80188f50 = order.v[data_80188f50];
}

void set_next_order_value_by_direction(Object *object, u8 index) {
    Quad order = data_8016d9a4;
    int step = object->slots[index].entry_index;

    game_state.step_value = step;
    if (object->slots[index].direction & 0x80) {
        game_state.step_value = step - 1;
    } else {
        game_state.step_value = step + 1;
    }
    data_80188f4c = order.v[game_state.step_value &= 3];
}

int call_slot_stage_handler_in_second_table(Object *object, u8 index, u8 arg) {
    u8 r;
    table_8017abd8[object->slots[index].stage](object, index, arg);
    r = slot_result_flag;
    return r;
}
