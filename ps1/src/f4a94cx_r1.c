/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* The local w holds the halfword read from the list and later link_flags for
   its test. With link_flags tested in place, this function differs from the
   original in 4 instruction slots. The halfword is put together again in two
   statements; in one statement: 13. Its high byte is stored through t; stored
   straight from w >> 8: 6, and 8 bytes shorter. It reaches w through t; read
   straight into w: 3, and 4 bytes shorter. */
void branch_script_by_linked_object_distance(Object *object) {
    u16 *p = data_80189460;
    Object *other;
    s16 w;
    u16 t;
    data_80189460 = p + 1;
    other = object->other;
    t = *p;
    w = t;
    t = w >> 8;
    object->limit_hi = t;
    object->limit_lo = w;
    w = other->link_flags;
    if (w == 0 || other->linked_object_3 == 0) {
        goto none;
    }
    ref_other.p = other->linked_object_3;
    if (ref_other.p->state_0 != 1) {
        goto none;
    }
    ref_second.p = other->linked_object_3;
    store_box_distance_to_other_x(object);
    t = object->limit_lo;
    t = t | (object->limit_hi << 8);
    if (t >= data_80189464) {
    none:
        read_script_word_and_dispatch(object);
    } else {
        object->field_209 = 7;
        object->field_208 = 0;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
}
