/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void check_range_hit_or_continue_sequence_slot04_04(Object *obj);
void update_sequence_or_finish_on_flag_slot04_04(Object *obj);
void func_801b4bc4_slot04_04(Object *obj);

void func_801b432c_slot04_04(Object *obj) {
    if (obj->state_3 == 0) {
        check_range_hit_or_continue_sequence_slot04_04(obj);
    } else {
        update_sequence_or_finish_on_flag_slot04_04(obj);
    }
}

void check_range_hit_or_continue_sequence_slot04_04(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index != 0 && obj->hit_flag_a != 0 && (u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        func_801b4bc4_slot04_04(obj);
    }
}
