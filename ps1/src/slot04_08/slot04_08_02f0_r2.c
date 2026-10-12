/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c3b60_slot04_08[];

void load_velocities_from_table_slot04_08(Object *obj) {
    update_sequence_with_input_check(obj);
}

void func_801b0458_slot04_08(Object *obj) {
    data_801c3b60_slot04_08[obj->state_3](obj);
}
