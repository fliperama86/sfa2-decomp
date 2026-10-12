/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c24f4_slot04_0b[])(Object *);

void load_velocities_from_table_3_slot04_0b(Object *obj) {
    update_sequence_with_input_check(obj);
}

void call_handler_by_state_3_set_flag_slot04_0b(Object *obj) {
    obj->field_157 = 1;
    data_801c24f4_slot04_0b[obj->state_3](obj);
}
