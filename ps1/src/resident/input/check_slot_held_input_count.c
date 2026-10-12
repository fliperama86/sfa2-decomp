/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 table_8017ab5c[];
void set_slot_level_from_repeat_count(Object *object, int index);

/*
 * Checks an object's flag halfword (input_word) against a table entry
 * (table_8017ab5c[arg]) (inferred, not original names). The entry's low
 * byte is the required-bit mask; entry bit 0x200 clear: the mask alone
 * decides; 0x200 and 0x100 set: flag bit 0 also passes; 0x200 set, 0x100
 * clear, 0x800 set: flag bit 1 also passes; none of these: nothing is
 * done. A pass increments the slot's repeat_count and calls clear_global_flag_ignoring_object; a
 * failure calls set_slot_level_from_repeat_count with the object and the slot index.
 */
/* The call of clear_global_flag_ignoring_object passes a second and a third argument although the callee takes one: the original has index and arg in the second and third argument registers at the call. Written with one argument, this function differs from the original in 22 instruction slots. */
void check_slot_held_input_count(Object *object, int index, int arg) {
    u16 entry;
    u16 flags;
    u16 *cnt;
    int bit0;
    int bit1;
    /* First the table entry, then its low byte. A separate local for the
       entry differs in 18 instruction lines. */
    u16 mask;

    arg &= 0xff;
    index &= 0xff;
    mask = table_8017ab5c[arg];
    entry = mask;
    flags = object->input_word;
    mask = entry & 0xff;
    /* Flag bits 0 and 1 are taken through one shared local (bit1). Taking
       them straight from flags differs in 26 instruction lines. */
    bit1 = flags;
    bit0 = bit1 & 1;
    bit1 = bit1 & 2;
    flags = flags & mask;
    /* Judgment call: repeat_count is a byte in Slot, the original increments a
       halfword there (lhu/addiu/sh). Without the cast the build warns. */
    cnt = (u16 *)&object->slots[index].repeat_count;
    if (!(entry & 0x200)) {
        if (mask != flags) {
            set_slot_level_from_repeat_count(object, index);
            return;
        }
        (*cnt)++;
        ((void (*)(Object *, int, int))clear_global_flag_ignoring_object)(object, index, arg);
        return;
    }
    if (entry & 0x100) {
        if (mask != flags && bit0 != 1) {
            set_slot_level_from_repeat_count(object, index);
        } else {
            (*cnt)++;
            ((void (*)(Object *, int, int))clear_global_flag_ignoring_object)(object, index, arg);
        }
        return;
    }
    if (entry & 0x800) {
        if (mask != flags && bit1 != 2) {
            set_slot_level_from_repeat_count(object, index);
        } else {
            (*cnt)++;
            ((void (*)(Object *, int, int))clear_global_flag_ignoring_object)(object, index, arg);
        }
    }
}
