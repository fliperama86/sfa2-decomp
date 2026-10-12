/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5250_slot04_06[];

void load_velocities_from_table_2_slot04_06(Object *obj) {
    update_sequence_with_input_check(obj);
}

void func_801b04d0_slot04_06(Object *obj) {
    data_801c5250_slot04_06[obj->state_3](obj);
}
