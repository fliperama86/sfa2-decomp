/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_sequence_by_side(Object *object, int arg);
void count_down_then_set_side_bit_slot04_0d(Object *object, GameState *g);

void update_sequence_with_random_loop_and_countdown_slot04_0d(Object *object) {
    int a;
    s8 r;

    if ((s16)object->step_flags < 0) {
        r = get_random_byte();
        a = r;
        if ((a & 3) != 0) {
            a = 0x32;
        } else {
            a = 0x33;
        }
        set_sequence_by_side(object, (u8)a);
    }
    count_down_then_set_side_bit_slot04_0d(object, &game_state);
    update_sequence(object);
}
