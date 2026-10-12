/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6594_slot04_02[];

void set_object_states_0_to_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d) {
    obj->state_0 = a;
    obj->state_1 = b;
    obj->state_2 = c;
    obj->state_3 = d;
}

void call_handler_by_input_dir_0_2_slot04_02(Object *obj) {
    data_801c6594_slot04_02[obj->input_dir_0 >> 1](obj);
}
