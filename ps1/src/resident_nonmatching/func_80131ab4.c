/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes (the build is 88 bytes
 * shorter) in register choice and in how the common tail of the first two
 * arms is shared. The build does not use this file. The differential test
 * next to it (difftest.py, with func_80131ab4.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not original names): appends to the input log
 * (input_log, a buffer of 0x1000 halfwords with a write index, the last
 * recorded word and a flag) the current state of the controller word of
 * the player that the object stands for. While the index is below 0xffd:
 *   - when the last recorded word equals data_801a6966 and the object's
 *     field_02 is 0, or equals data_801a6972 and field_02 is 1 (a repeat of
 *     the same input), the log gets the word with bit 0x200 set at the
 *     index and the repeat counter data_80171b34 at the index + 1; the
 *     same marked word goes to data_80171b32, the counter is incremented
 *     and the flag is set to 1 (the index does not move);
 *   - otherwise, if the flag is set, the index moves on by 2 (past the
 *     counter entry); the counter is cleared; the controller word of the
 *     object's side (data_801a6972 when the side is not 0, else
 *     data_801a6966) is written at the index and becomes the last recorded
 *     word; the flag is cleared; the index moves on by 1.
 * With the index at 0xffd or more nothing happens.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: input_log (index, last, flag, and the buffer is only written),
 *     data_801a6966, data_801a6972, data_80171b34, the object's field_02 and
 *     side.
 *   Writes: input_log (buffer entries, index, last, flag), data_80171b32,
 *     data_80171b34.
 *   Aliasing: the object is one block; all else is the fixed data.
 *   Excluded: none (an index above 0xffc is tried and writes nothing).
 *   Slots no input can reach: none known.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Declared here because the published headers lack it (inferred type). */
extern u16 data_801a6966;

void func_80131ab4(Object *object) {
    u16 word;
    u16 count;
    u16 *src;

    if (input_log.index >= 0xffd) return;
    if ((input_log.last == data_801a6966 && object->field_02 == 0) ||
        (input_log.last == data_801a6972 && object->field_02 == 1)) {
        word = input_log.last | 0x200;
        count = data_80171b34;
        data_80171b32 = word;
        input_log.buf[input_log.index] = word;
        input_log.buf[input_log.index + 1] = count;
        data_80171b34 = count + 1;
        input_log.flag = 1;
    } else {
        if (input_log.flag != 0) input_log.index += 2;
        data_80171b34 = 0;
        src = object->side != 0 ? &data_801a6972 : &data_801a6966;
        input_log.buf[input_log.index] = *src;
        input_log.last = *src;
        input_log.flag = 0;
        input_log.index++;
    }
}
