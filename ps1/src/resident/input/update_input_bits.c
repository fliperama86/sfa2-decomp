/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* What it does (inferred, not original names): per-frame update of an object's button words. When game_state.versus_flag is not 0 it checks the object's side against game_state.mode: on a mismatch it clears the object's input_bits, input_bits_2 and input_word, and sets all three to 0x4000 when the flag byte data_801a6984[3] is not 0; on a match it calls record_input_log(object) when data_801a6984[1] is not 0. Then, when facing_side is not 0, it swaps bits 0x2000 and 0x8000 of input_bits_2 (bit 13 moves to bit 15 and bit 15 to bit 13); when saved_facing is not 0 it does the same for input_word. Last it stores in pressed_bits the bits of input_bits_2 not in prev_input_bits_2, and in released_bits the bits of prev_input_bits_2 not in input_bits_2 (the pressed and the released bits). */
void update_input_bits(Object *object) {
    unsigned a, b, c, d;
    u16 x;
    s16 y;
    /* The flag bytes are read as elements of data_801a6984 (the headers name [1] and [3] separately as data_801a6985 and data_801a6987 as well; this is the same address). */
    u8 *g = &game_state.versus_flag;
    if (*g != 0) {
        /* Stand-in, not recovered source: the read of game_state.mode, written as the pointer g moved back by the distance between versus_flag and mode (the same address; the original reads mode at -0x15 from the address of versus_flag that it holds in a register), reproduces the original's choice of registers. Without it this function differs from the original in 17 instruction slots. */
        if (*(g - (&game_state.versus_flag - &game_state.mode)) != object->side + 1) {
            object->input_bits = 0;
            object->input_bits_2 = 0;
            object->input_word = 0;
            if (data_801a6984[3] != 0) {
                object->input_bits = 0x4000;
                object->input_bits_2 = 0x4000;
                object->input_word = 0x4000;
            }
        } else if (data_801a6984[1] != 0) {
            record_input_log(object);
        }
    }
    x = object->input_bits_2;
    y = object->input_word;
    a = (x >> 2) & 0x2000;
    b = (x & 0x2000) << 2;
    c = (y & 0x8000) >> 2;
    d = (y & 0x2000) << 2;
    if (object->facing_side != 0) {
        object->input_bits_2 = (x & 0x5fff) + (a + b);
    }
    if (object->saved_facing != 0) {
        object->input_word = (object->input_word & 0x5fff) + (c + d);
    }
    /* Stand-in, not recovered source: the local a, used again for the two last results after serving as a mask above, reproduces the original's choice of registers. Without it this function differs from the original in 22 instruction slots. */
    /* It changes no value. The 22 slots are with plain expressions in its place. */
    a = object->prev_input_bits_2;
    a = ~a;
    a &= object->input_bits_2;
    object->pressed_bits = a;
    a = object->input_bits_2;
    a = ~a;
    a &= object->prev_input_bits_2;
    object->released_bits = a;
}
