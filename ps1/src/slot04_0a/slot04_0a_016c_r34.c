/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c0884_slot04_0a[];

void func_801b49c8_slot04_0a(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}

void update_sequence_or_finish_on_flag_4_slot04_0a(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b4a68_slot04_0a(Object *obj) {
    data_801c0884_slot04_0a[obj->state_3](obj);
}
