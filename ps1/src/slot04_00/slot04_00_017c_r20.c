/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c056c_slot04_00[];

void func_801b3314_slot04_00(Object *o) {
    call_state_2_handler_2684_unless_paused(o);
}

void load_velocities_from_table_4_slot04_00(Object *obj) {
    call_state_2_handler_268c_unless_paused(obj);
}

void load_velocities_from_table_5_slot04_00(Object *obj) {
    call_handler_2694_by_state_2(obj);
}

void func_801b3374_slot04_00(Object *o) {
    data_801c056c_slot04_00[o->state_1](o);
    func_8011ffdc(o);
}
