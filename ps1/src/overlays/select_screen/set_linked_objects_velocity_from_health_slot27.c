/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern s16 data_80026a58_slot27[];

void set_linked_objects_velocity_from_health_slot27(Object *obj) {
    int t;
    int u;
    scratch_word_a[0] = 0;
    scratch_word_b[0] = 1;
    scratch_word_c[0] = 0;
    if ((s16)obj->health >= 9) {
        scratch_word_a[0] = 1;
        scratch_word_b[0] = 0;
        scratch_word_c[0] = 1;
    }
    ref_first.p = (Object *)((ObjectWords *)obj)->word_54;
    ref_second.p = (Object *)((ObjectWords *)obj)->word_58;
    ref_first.p->field_62 = scratch_word_a[0];
    ref_second.p->field_62 = scratch_word_b[0];
    if (obj->field_62 != scratch_word_c[0]) {
        obj->field_62 = scratch_word_c[0];
    }
    scratch_word_b[0] = (s16)obj->health;
    t = data_80026a58_slot27[scratch_word_b[0]];
    scratch_word_a[0] = t;
    u = data_80026a58_slot27[scratch_word_b[0] + 1];
    scratch_word_b[0] = u;
    if (ref_other.p->side != 0) {
        scratch_word_a[0] = -t;
    }
    ref_first.p->vel_x = scratch_word_a[0];
    scratch_word_a[0] = -scratch_word_a[0];
    ref_second.p->vel_x = scratch_word_a[0];
    ref_first.p->vel_y = scratch_word_b[0];
    scratch_word_b[0] = -scratch_word_b[0];
    ref_second.p->vel_y = scratch_word_b[0];
}
