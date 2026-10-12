/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void check_input_match_or_advance_slot_stage(Object *object, int index, int arg) {
    u16 entry = table_8017aaf8[(u8)arg * 7 + 1];
    int mine = entry & -0x1000;
    int theirs = object->input_word & -0x1000;

    if (!(entry & 0x400)) {
        if ((s16)theirs != (s16)mine) goto other;
    } else {
        if ((theirs & mine) == 0) goto other;
    }
    clear_global_flag_ignoring_object(object);
    return;
other:
    object->slots[(u8)index].stage++;
    set_slot_random_timer_and_advance(object, (u8)index, (u8)arg);
    check_slot_high_input_bits_and_advance(object, (u8)index, (u8)arg);
}
