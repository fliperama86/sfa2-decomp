/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void start_hit_sequence_base_7_or_4(Object *object) {
    u16 base;
    int step;
    object->field_60 = 0;
    object->countdown = 0;
    object->facing_side = object->saved_facing;
    base = 7;
    if (object->field_157 == 0) base = 4;
    step = object->field_62;
    if (step >= 2) step = 2;
    base += step;
    set_sequence_by_side_and_mode_2(object, base);
    move_other_by_kind_table_on_hit(object);
    if (object->field_6b == 0) func_8012cca8(object);
}

void set_sequence_from_hit_type_base_and_capped_step(Object *object) {
    u16 base;
    int step;
    object->field_60 = 0;
    object->countdown = 0;
    object->field_157 = 0;
    object->facing_side = object->saved_facing;
    base = 1;
    if (object->hit_type == 0x18) base = 4;
    step = object->field_62;
    if (step >= 2) step = 2;
    base += step;
    set_sequence_by_side_and_mode_2(object, base);
    if (object->field_6b == 0) func_8012cca8(object);
}
