/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void queue_bcd_value_texts(void) {
    u32 *p = &data_8018d270;
    *p = game_state.bcd_amount;
    if (data_8018d26a != 0) {
        format_bcd_text(&data_80181668, data_8018d26c << 8);
        format_bcd_text(&data_80181684, data_8018d269);
        format_bcd_text(&data_801816a0, data_8018d274 << 8);
        format_bcd_text(&data_801816bc, *p << 8);
        add_item_to_table(&data_801816d4);
        add_item_to_table(&data_80181708);
        add_item_to_table(&data_8018171c);
        add_item_to_table(&data_80181758);
    } else {
        format_bcd_text(&data_80181684, data_8018d26c << 8);
        format_bcd_text(&data_801816a0, data_8018d269);
        format_bcd_text(&data_801816bc, *p << 8);
        add_item_to_table(&data_801816e4);
        add_item_to_table(&data_8018172c);
        add_item_to_table(&data_8018173c);
    }
    add_item_to_table(&data_80181774);
    add_item_to_table(&data_8018178c);
}

void noop_empty_function_17(void) {
}
