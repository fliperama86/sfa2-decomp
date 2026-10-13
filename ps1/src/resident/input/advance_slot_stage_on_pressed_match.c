/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* The masked index is held twice: in the byte local slot, which the second
   half of the function uses, and written back to the parameter, which the
   first half uses. With slot everywhere and no write back, this function
   differs from the original in 16 instruction slots; with the parameter
   masked in place and used everywhere, in 4. The last argument of the third
   call goes through f: with (u8)arg passed there it differs in 5. */
void advance_slot_stage_on_pressed_match(Object *object, int index, int arg) {
    u8 slot = index;
    u16 entry;
    u16 m;
    u16 f;
    index = slot;
    object->slots[index].entry_index = 0;
    entry = slot_input_table[(u8)arg * 7];
    f = object->pressed_bits;
    m = entry & 0xf0ff;
    if ((f & m) == 0 || (!(entry & 0x400) && m != (object->input_bits_2 & 0xf0ff))) {
        reset_slot_stage_clear_result(object, index, (u8)arg);
    } else {
        object->slots[slot].stage++;
        if (entry & 1) {
            reset_slot_stage_set_result(object, slot, (u8)arg);
        } else {
            f = arg;
            set_slot_random_timer_and_advance(object, slot, (u8)f);
        }
    }
}
