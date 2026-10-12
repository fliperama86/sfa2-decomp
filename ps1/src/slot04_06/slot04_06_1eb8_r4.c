/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_state_with_countdown_2_and_sound_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    o->state_3++;
    obj->countdown_2 = 0x32;
    func_80145d20(o);
    func_801428e4(o);
    set_flag_225_add_direction_to_counter(&game_state, o);
    play_side_sound_at(o, o->side, 7);
    set_sequence_by_side_and_mode(o, 0x22);
}
