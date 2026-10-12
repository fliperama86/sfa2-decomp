/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void update_slot_three_input_counters(Object *object, int index, int arg) {
    Slot *slot = &object->slots[(u8)index];
    if ((u8)update_input_counter_pair_with_limit(&slot->entry_index, table_8017ab6c[(u8)arg * 7] & 0xc0, ((s16 *)table_8017ab6c)[(u8)arg * 7 + 1], ((s16 *)table_8017ab6c)[(u8)arg * 7 + 2], object->pressed_bits) == 0
        && (u8)update_input_counter_pair_with_limit(&slot->entry_index + 2, table_8017ab6c[(u8)arg * 7] & 0x30, ((s16 *)table_8017ab6c)[(u8)arg * 7 + 3], ((s16 *)table_8017ab6c)[(u8)arg * 7 + 4], object->pressed_bits) == 0
        && (u8)update_input_counter_pair_with_limit(&slot->entry_index + 4, table_8017ab6c[(u8)arg * 7] & 0xc, ((s16 *)table_8017ab6c)[(u8)arg * 7 + 5], ((s16 *)table_8017ab6c)[(u8)arg * 7 + 6], object->pressed_bits) == 0) {
        clear_global_flag_ignoring_object(object);
    } else {
        data_80188f44 = 1;
    }
}
