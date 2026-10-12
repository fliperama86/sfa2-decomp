/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b7ebc_slot04_02(Object *obj);

void set_state_0_to_2_slot04_02(Object *obj) {
    obj->state_0 = 2;
}

void func_801b7d64_slot04_02(Object *obj) {
    int t;

    if (game_state.field_64 != 0) {
        obj->accel_x = obj->accel_x - 1;
        if (obj->accel_x == 0 && (s16)obj->countdown != 0) {
            func_801b7ebc_slot04_02(obj);
            t = obj->countdown - 1;
            obj->countdown = t;
            if ((s16)t != 0) {
                obj->accel_x = 6;
            } else {
                obj->countdown = 0xfff;
            }
        }
        advance_sequence_checked(obj);
        clear_or_set_flag_by_pos_x_limit(obj);
    } else {
        obj->state_0++;
    }
}
