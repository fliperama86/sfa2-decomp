/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0978_slot04_05(Object *obj);
void func_801b0a08_slot04_05(Object *obj);
extern ObjectFn data_801c1388_slot04_05[];

void load_velocities_from_table_2_slot04_05(Object *obj) {
    update_sequence_with_input_check(obj);
}

void func_801b0938_slot04_05(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        func_801b0a08_slot04_05(obj);
    } else {
        func_801b0978_slot04_05(obj);
    }
}

void func_801b0978_slot04_05(Object *obj) {
    data_801c1388_slot04_05[obj->state_3](obj);
}
