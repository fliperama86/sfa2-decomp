/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/*
 * What it does (inferred, not original names): steps one numbered slot of
 * an object. It decrements the slot's timer; when that reaches 0 it
 * calls reset_slot_stage_clear_result and returns. Otherwise it takes the table entry
 * table_8017a8cc[arg * 7 + slot.entry_index], the object's two input halfwords
 * ORed together (pad) and the pad's low two bits (dir), and matches the
 * entry's mask (its low byte). Entry bit 0x200 clear: a hit when pad and
 * mask share a bit of 0xfc, or the mask has a bit of 0x94 and dir is 1, or
 * a bit of 0x68 and dir is 2. Entry bit 0x200 set: with m = pad & mask, a
 * hit when entry bit 0x100 is set and (m is 0x94 or dir is 1), or else when
 * entry bit 0x800 is set and (m is 0x68 or dir is 2). A hit calls
 * reset_slot_stage_set_result, anything else func_8013f2c8.
 */
void check_slot_input_by_entry_index(Object *object, u8 index, u8 arg) {
    u16 mask;
    u16 entry;
    u16 pad;
    u16 dir;

    object->slots[index].timer--;
    if (object->slots[index].timer == 0) {
        reset_slot_stage_clear_result(object, index, arg);
        return;
    }
    mask = table_8017a8cc[arg * 7 + object->slots[index].entry_index];
    pad = object->pressed_bits | object->released_bits;
    dir = pad & 3;
    /* mask holds the whole table entry first and its low byte from here on: with a local of its own for
       the entry this function differs from the original in 20 instruction lines. */
    entry = mask;
    mask = entry & 0xff;
    /* Each arm has its own call: with the conditions of arms that call the same thing joined by ||,
       this function differs from the original in 14 to 35 instruction lines. */
    if (!(entry & 0x200)) {
        if (pad & mask & 0xfc)
            reset_slot_stage_set_result(object, index, arg);
        else if ((mask & 0x94) && dir == 1)
            reset_slot_stage_set_result(object, index, arg);
        /* 0x868 as in the original: mask has 8 bits, so 0x68 selects the same bits, but with 0x68 this
           function differs from the original in 1 instruction line. */
        else if ((mask & 0x868) && dir == 2)
            reset_slot_stage_set_result(object, index, arg);
        else
            func_8013f2c8(object);
        return;
    }
    /* The & 0xff does not change the value (mask is at most 0xff); without it this function differs
       from the original in 1 instruction line. */
    mask &= pad & 0xff;
    if ((entry & 0x100) && mask == 0x94)
        reset_slot_stage_set_result(object, index, arg);
    else if ((entry & 0x100) && dir == 1)
        reset_slot_stage_set_result(object, index, arg);
    else if (!(entry & 0x800))
        func_8013f2c8(object);
    else if (mask == 0x68)
        reset_slot_stage_set_result(object, index, arg);
    else if (dir == 2)
        reset_slot_stage_set_result(object, index, arg);
    else
        func_8013f2c8(object);
}
