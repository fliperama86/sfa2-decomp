/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079938_slot2b[];
extern s16 data_8007ef24_slot2b;

void load_velocities_from_table_slot2b(Object *obj) {
    call_state_2_handler_267c_unless_paused(obj);
}

void load_velocities_from_table_2_slot2b(Object *obj) {
    call_state_2_handler_2684_unless_paused(obj);
}
