/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void func_801b4bc4_slot04_04(Object *obj);
extern ObjectFn data_801c45cc_slot04_04[];

void advance_state_3_restore_facing_slot04_04(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = obj->saved_facing;
    func_801b4bc4_slot04_04(obj);
}

void update_sequence_or_finish_on_flag_3_slot04_04(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b4678_slot04_04(Object *obj) {
    data_801c45cc_slot04_04[obj->state_3](obj);
}
