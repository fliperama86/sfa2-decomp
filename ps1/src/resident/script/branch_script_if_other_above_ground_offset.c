/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void branch_script_if_other_above_ground_offset(Object *object) {
    u16 v = *data_80189460++;
    Object *other = object->other;
    if ((u16)(other->ground_y - v) >= (u16)other->pos_y) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}

void branch_script_if_other_link_entry_is_0_or_4(Object *object) {
    Object *ref;
    u8 v;
    if (object->other->link_flags == 0 || (ref = (Object *)object->other->linked_object_3) == 0
        || (v = table_8017d0ac[(ref->field_02 << 4) + (ref->vel_table_index >> 1)]) == 0 || v == 4) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}

void branch_script_if_other_link_entry_ge_3(Object *object) {
    Object *ref;
    if (object->other->link_flags == 0 || (ref = (Object *)object->other->linked_object_3) == 0
        || table_8017d0ac[(ref->field_02 << 4) + (ref->vel_table_index >> 1)] < 3) {
        skip_script_block_then_read_opcode(object);
    } else {
        read_script_word_and_dispatch(object);
    }
}

void branch_script_if_own_health_below_value(Object *object) {
    s16 v = *(s16 *)data_80189460++;
    if ((s16)object->health < v) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}

void branch_script_on_random_one_in_four(Object *object) {
    if ((get_random_byte() & 3) == 0) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}
