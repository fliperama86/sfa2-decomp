/* Reconstruction. Names/roles inferred, not original symbols. */
/* Exact: the last call is written once per path (early-return arm). */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: orig keeps arg in place in $a2 and the slot pointer in $a1. */
void update_slot_timer_match_input_top_bits(Object *object, int index, int arg) {
    u16 entry;
    int mine;
    int theirs;

    Object *slot;

    arg &= 0xff;
    entry = table_8017aaf8[arg * 7 + 1];
    mine = entry & -0x1000;
    theirs = object->input_word & -0x1000;

    if (!((entry & 0x400) ? (theirs & mine) != 0 : (s16)mine == (s16)theirs)) {
        reset_slot_stage_clear_result(object, (u8)index, arg);
    } else {
        u8 *p = (u8 *)object + ((u8)index << 3);
        p[0x2b4]--;
        if (p[0x2b4] != 0) {
            clear_global_flag_ignoring_object(object);
            return;
        }
        p[0x2b0]++;
        p[0x2b1]++;
        clear_global_flag_ignoring_object(object);
    }
}
