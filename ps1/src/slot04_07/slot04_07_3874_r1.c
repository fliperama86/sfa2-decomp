/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c20a8_slot04_07[];
void func_801b3e8c_slot04_07(Object *obj);

void move_x_by_variant_index_with_decel_stop_slot04_07(Object *obj) {
    if (obj->variant_index == 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
}

void call_handler_by_input_direction_slot04_07(Object *obj) {
    if (obj->input_dir_0 == 4) {
        func_801b3e8c_slot04_07(obj);
    } else {
        data_801c20a8_slot04_07[obj->input_dir_second >> 1](obj);
    }
}
