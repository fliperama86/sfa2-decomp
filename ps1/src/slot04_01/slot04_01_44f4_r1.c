/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1138_slot04_01(Object *obj);

void func_801b44f4_slot04_01(Object *obj) {
    int a = 0xc;
    int one;

    if (obj->limit_lo & 1) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 3;
        obj->active_flag_3 = 1;
        func_801b1138_slot04_01(obj);
        return;
    }
    obj->state_3 = 3;
    one = 1;
    obj->active_flag_3 = one;
    if (obj->hit_flag_a != 0 && obj->pos_y - obj->ground_y < -0x2f
        && (check_hit_other_active_state_nonzero(obj, -0x20, 0x20, 0, 0x10) & 0xff)) {
        obj->state_1 = 2;
        obj->state_0 = one;
        obj->state_2 = 0;
        obj->state_3 = 0;
        obj->input_dir_0 = 4;
        return;
    }
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    if (obj->variant_index != 0) {
        a = 0x12;
    }
    if (obj->input_dir_second != 0) {
        a += 3;
    }
    set_sequence_by_side_and_mode(obj, (s16)((obj->direction_index >> 1) + a));
}
