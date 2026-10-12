/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012c5cc(Object *object) {
    int step;
    u16 index;
    object->facing_side = object->saved_facing;
    object->field_60 = 0;
    object->countdown = 0;
    if (object->field_157 != 0) {
        step = object->field_62;
        if (step >= 2) {
            step = 2;
        }
        index = step + 7;
        set_sequence_by_side_and_mode_2(object, index);
    } else if (object->hit_type == 8 || object->hit_type == 0) {
        step = object->field_62;
        if (step >= 2) {
            step = 2;
        }
        index = step + 1;
        set_sequence_by_side_and_mode_2(object, index);
    } else {
        step = object->field_62;
        if (step >= 2) {
            step = 2;
        }
        index = step + 4;
        set_sequence_by_side_and_mode_2(object, index);
    }
    move_other_by_kind_table_on_hit(object);
    if (object->field_6b == 0) {
        func_8012cca8(object);
    }
}
