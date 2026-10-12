/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c6568_slot04_02[];
extern u16 data_801c6578_slot04_02[];

void restore_facing_and_set_sequence_0_slot04_02(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void advance_state_2_when_timers_clear_slot04_02(Object *obj) {
    if (game_state.field_64 == 0 && game_state.countdown_timer == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}

void set_random_sequence_from_table_slot04_02(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->countdown = 0x3c;
    game_state.countdown = 0x1e;
    set_sequence_by_side(obj, data_801c6578_slot04_02[(u8)get_input_mapped_value_or_default(obj, data_801c6568_slot04_02[get_random_byte() & 0xf])]);
}
