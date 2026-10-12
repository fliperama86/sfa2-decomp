/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_801c3ea4_slot04_08;
extern SequenceStep **data_801c3ea8_slot04_08;

void func_801b4ac0_slot04_08(Object *obj, Object *other) {
    u8 b;
    u16 h;

    obj->state_0++;
    obj->field_1c = other->field_1c;
    obj->ot_depth = 3;
    obj->field_0c = other->field_0c;
    obj->image_row = other->image_row;
    obj->field_0e = other->field_0e;
    b = other->facing_side;
    obj->variant_index = 0;
    obj->countdown = 0;
    obj->facing_side = b;
    ref_other.p = obj->parent;
    h = ref_other.p->field_1c;
    obj->spawn_mode = 6;
    obj->field_1c = h;
    if (other->side == 0) {
        data_801c3ea4_slot04_08 = data_1f8000b4;
    } else {
        data_801c3ea8_slot04_08 = data_1f800164;
    }
    if (other->side == 0) {
        set_sequence_from_table_5(obj, (u8)(obj->table_index + 8), data_801c3ea4_slot04_08);
    } else {
        set_sequence_from_table_5(obj, (u8)(obj->table_index + 8), data_801c3ea8_slot04_08);
    }
    obj->field_01 = 1;
}
