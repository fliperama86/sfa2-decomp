/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* What it does (inferred, not original names): a sibling of check_slot_input_by_entry_index,
   with the table halfword fixed at step 1 and the pad compared as a whole.
   It decrements the slot's timer; when that reaches 0 it calls
   reset_slot_stage_clear_result and returns. Otherwise it takes the table halfword
   slot_input_table[arg * 7 + 1], the object's two input halfwords ORed
   together (bits, pressed_bits | released_bits) and the entry's mask (its low
   byte, q):
     - entry bit 0x200 clear: a hit (reset_slot_stage_set_result) when the pad and mask
       share a bit among 0xfc, or the mask has a bit of 0x94 and the pad is
       exactly 1, or it has a bit of 0x68 and the pad is exactly 2 (the
       original tests the mask against 0x868; the mask has 8 bits);
       otherwise clear_global_flag_ignoring_object;
     - entry bit 0x200 set, with m = pad & mask: when entry bit 0x100 is set
       and m is 0x94, reset_slot_stage_set_result and return; when bit 0x100 is set and
       the pad is exactly 1, reset_slot_stage_set_result is called and the function goes
       on; then, when entry bit 0x800 is set and (m is 0x68 or the pad is
       exactly 2), reset_slot_stage_set_result, else clear_global_flag_ignoring_object. */
void check_slot_input_against_table_entry(Object *object, u8 index, u8 arg) {
    u16 entry;
    u16 q;
    u16 m;
    u16 bits;

    object->slots[index].timer--;
    if (object->slots[index].timer == 0) {
        reset_slot_stage_clear_result(object, index, arg);
        return;
    }
    entry = slot_input_table[(arg * 7) + 1];
    /* Stand-in, not recovered source: the assignment below, which nothing reads (m is set again three statements down), reproduces the original's choice of registers. Without it this function differs from the original in 16 instruction slots. */
    m = entry;
    bits = object->pressed_bits | object->released_bits;
    q = entry & 0xff;
    /* Stand-in, not recovered source: the local m as a second name of q, read once in the third test below, reproduces the original's choice of registers. Without it this function differs from the original in 35 instruction slots. */
    m = q;
    if (!(entry & 0x200)) {
        if ((bits & q) & 0xfc) {
            reset_slot_stage_set_result(object, index, arg);
        } else if ((q & 0x94) && (bits == 1)) {
            reset_slot_stage_set_result(object, index, arg);
        } else if ((m & 0x868) && (bits == 2)) {
            reset_slot_stage_set_result(object, index, arg);
        } else {
            clear_global_flag_ignoring_object(object);
        }
    } else {
        m = bits & q;
        if (entry & 0x100) {
            if (m == 0x94) {
                reset_slot_stage_set_result(object, index, arg);
                return;
            }
            if (bits == 1) {
                reset_slot_stage_set_result(object, index, arg);
            }
        }
        if ((entry & 0x800) && ((m == 0x68) || (bits == 2))) {
            reset_slot_stage_set_result(object, index, arg);
        } else {
            clear_global_flag_ignoring_object(object);
        }
    }
}
