/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c4164_slot04_04[];

void restore_facing_and_set_sequence_0_slot04_04(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void update_sequence_advance_state_2_on_timer_slot04_04(Object *obj) {
    if (game_state.field_64 == 0 || game_state.countdown_timer == 0) {
        obj->state_2++;
    }
    update_sequence(obj);
}

void start_random_sequence_set_countdown_slot04_04(Object *obj) {
    s16 k;

    obj->countdown = 0x3c;
    obj->state_2++;
    game_state.countdown = 0x1e;
    k = 3;
    if ((s16)obj->health != 0x90) {
        k = data_801c4164_slot04_04[get_random_byte() & 0xf];
    }
    k = (s8)get_input_mapped_value_or_default(obj, (s8)k);
    k += 0x23;
    set_sequence_by_side(obj, k);
}

void count_down_and_set_side_bit_slot04_04(Object *obj) {
    s16 t = obj->countdown;
    if (t != 0) {
        t = t - 1;
        obj->countdown = t;
        if (t == 0) {
            game_state.side_bits_2 |= 1 << obj->side;
        }
    }
    update_sequence(obj);
}
