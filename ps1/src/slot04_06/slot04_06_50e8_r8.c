/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_spawned_object_rising_sequence_slot04_06(Object *obj) {
    obj->spawn_flag = 1;
    obj->field_0c = 0;
    obj->image_row = 0;
    obj->vel_y = 0x40000;
    obj->state_0++;
    set_sequence_from_table_5(obj, 0x23, seqs_8017c7f8);
}

void rise_by_vel_y_and_advance_sequence_slot04_06(Object *obj) {
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    if ((s16)obj->step_flags < 0) {
        obj->state_0++;
    }
    advance_sequence_checked(obj);
    func_8011ffdc(obj);
}
