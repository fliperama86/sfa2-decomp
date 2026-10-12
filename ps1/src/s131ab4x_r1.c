/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* Declared here because the published headers lack it (inferred type). */
extern u16 data_801a6966;

/*
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
 */
void record_input_log(Object *object) {
    u16 n;
    u16 n1;
    if (input_log.index < 0xffd) {
        if (input_log.last == data_801a6966 && object->field_02 == 0) {
            n = data_80171b34;
            n1 = n + 1;
            data_80171b32 = (input_log.last | 0x200);
            input_log.buf[input_log.index] = (input_log.last | 0x200);
            input_log.buf[input_log.index + 1] = n;
            data_80171b34 = n1;
            input_log.flag = 1;
        } else if (input_log.last == data_801a6972 && object->field_02 == 1) {
            u16 v2;
            n = data_80171b34;
            n1 = n + 1;
            v2 = input_log.last | 0x200;
            input_log.buf[input_log.index] = v2;
            data_80171b32 = v2;
            input_log.buf[input_log.index + 1] = n;
            data_80171b34 = n1;
            input_log.flag = 1;
        } else {
            u16 *src;
            /* One local for two values (the flag address, then the buffer address): written with separate values this function differs from the original in 22 instruction slots. */
            u8 *pf = &input_log.flag;
            if (*pf != 0) {
                input_log.index += 2;
            }
            data_80171b34 = 0;
            if (object->side != 0) {
                src = &data_801a6972;
                pf = (u8 *)input_log.buf;
            } else {
                pf = (u8 *)input_log.buf;
                src = &data_801a6966;
            }
            ((u16 *)pf)[input_log.index] = *src;
            /* Stand-in, not recovered source: the test below, whose condition is 0 for every value of src, reproduces the original's order of instructions. Without it (the plain statement input_log.last = *src;) this function differs from the original in 13 instruction slots. */
            if (((u32)(unsigned long long)src << 24) & 0x100) {
                n = 0;
            } else {
                input_log.last = *src;
            }
            input_log.index++;
            input_log.flag = 0;
        }
    }
}
