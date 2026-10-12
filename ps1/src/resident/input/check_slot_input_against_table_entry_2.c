/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 table_8017ab5c[];
void advance_slot_stage_with_timer(Object *object, int index, int arg);

/*
 * What it does (inferred, not original names): checks an object's flag
 * halfword (input_word) against a table entry (table_8017ab5c[arg]), and
 * calls advance_slot_stage_with_timer when the check passes, reset_slot_stage_clear_result when it fails.
 * The entry's low byte is a required-bit mask. Entry bit 0x200 clear: the
 * mask alone decides. Bit 0x200 set with 0x100 set: the check also passes
 * when input_word bit 0 is set. Bit 0x200 set, 0x100 clear, 0x800 set: it also
 * passes when input_word bit 1 is set. Otherwise it does nothing.
 */
void check_slot_input_against_table_entry_2(Object *object, int index, int arg) {
    u8 slot;
    int i;
    u16 ent;
    u16 flags;
    int f;
    int b0;
    u16 t;
    arg &= 0xff;
    /* The original copies the table value and the index before masking them. */
    t = table_8017ab5c[arg];
    ent = t;
    t = ent & 0xff;
    flags = object->input_word;
    f = flags;
    /* The index is narrowed to a byte in slot and held once more in i: with slot alone 20 instruction lines differ, with i alone 16. */
    slot = index;
    i = slot;
    b0 = f & 1;
    f = f & 2;
    flags &= t;
    if (!(ent & 0x200)) {
        if (t == flags)
            advance_slot_stage_with_timer(object, i, arg);
        else
            reset_slot_stage_clear_result(object, i, arg);
        return;
    }
    if (ent & 0x100) {
        if (t != flags && b0 != 1)
            reset_slot_stage_clear_result(object, i, arg);
        else
            advance_slot_stage_with_timer(object, slot, arg);
        return;
    }
    if (ent & 0x800) {
        if (t != flags && f != 2)
            reset_slot_stage_clear_result(object, i, arg);
        else
            advance_slot_stage_with_timer(object, slot, arg);
    }
}
