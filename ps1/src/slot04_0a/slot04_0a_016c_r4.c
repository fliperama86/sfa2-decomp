/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c0550_slot04_0a[];

void load_velocities_from_table_slot04_0a(Object *obj) {
    update_sequence_with_input_check(obj);
}

void func_801b07cc_slot04_0a(Object *obj) {
    data_801c0550_slot04_0a[obj->state_3](obj);
}
