/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_state_2_when_timer_zero_slot04_02(Object *object) {
    if (game_state.countdown_timer == 0) {
        object->state_2 = object->state_2 + 1;
    }
    update_sequence(object);
}

void start_sequence_by_side_with_countdown_slot04_02(Object *obj) {
    int idx = 0x28;
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->other->facing_side;
    obj->countdown = 0x78;
    if (game_state.field_a6 != 0) {
        idx = 0x29;
    }
    set_sequence_by_side(obj, idx);
}

void count_down_and_set_side_bit_slot04_02(Object *obj) {
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
