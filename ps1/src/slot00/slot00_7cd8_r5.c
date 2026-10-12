/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_8007a0ac_slot00[];
void set_sequence_by_sound_side_slot00(Object *obj, int mode);

void set_sequence_four_by_sound_side_slot00(Object *obj) {
    obj->ot_depth = 0;
    if (obj->sound_side == 0) {
        obj->sequence = seqs_a4_left[4];
    } else {
        obj->sequence = seqs_154_right[4];
    }
    obj->step_timer = obj->sequence->duration;
    obj->step_flags = obj->sequence->flags;
    obj->sequence_changed_flag = 1;
}

void set_sequence_index_0_slot00(Object *obj) {
    set_sequence_by_sound_side_slot00(obj, 0);
}
