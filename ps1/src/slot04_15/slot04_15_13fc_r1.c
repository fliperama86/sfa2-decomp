/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);

void start_chained_sequence_state_3_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    o->active_flag_4 = 1;
    o->state_3++;
    add_to_meter_clamped(o, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, o);
    obj->active_flag_2 = 0;
    obj->table_index_3 = 0;
    set_sequence_by_side_and_mode(o, 0x27);
}
