/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 4 bytes longer (it forms the record address differently).
 * The PS1 build keeps the raw bytes of the module image and does not
 * use this file. The differential test next to it (difftest.py, with
 * func_801b3a70_slot04_05.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): steps one entry of the
 * object's table of 8-byte records (the table at 0x2b0, index a1). A mask
 * is looked up in a table of 6-byte rows, row a2, second halfword of the
 * row (the table starts at data_801c1630_slot04_05 + 12). When the object's
 * halfword field_134 has a bit of that mask: the byte at record offset 2 is
 * decremented; if it is then non-zero the byte at offset 4 is set to 0xc,
 * otherwise the word data_801c1700_slot04_05 is set to -1. When no bit
 * matches: the byte at offset 4 is decremented and, if it reaches 0, the
 * byte at offset 0 is cleared.
 *
 * Contract:
 *   Arguments: a0 = pointer to an object; a1 = record index, a2 = row
 *     index, both used as their low byte. No return value.
 *   Reads: field_134, the halfword at row a2 of the mask table, bytes 2
 *     and 4 of the record.
 *   Writes: byte 2 and byte 4 of the record, byte 0 of the record, and the
 *     word data_801c1700_slot04_05, each in the arm described.
 *   Callees: none.
 *   Aliasing: the object is one block; the record index is not bounded by
 *     the original (up to 255, 8 bytes each), so the setup makes the block
 *     large enough for every index. The mask table is the module's data
 *     itself: the setup writes the rows 0 to 30 with random masks (row 30
 *     ends below data_801c1700_slot04_05) and uses a2 in that range.
 *   Inputs excluded: a2 above 30 (the table has no more rows before the
 *     word the function writes; the original reads whatever data follows).
 *   Slots not reached: none (all 41 slots are executed).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A record of the table at 0x2b0 of the object, 8 bytes: a state byte,
   a counter at offset 2 and a timer at offset 4. Inferred; the header
   declares offset 2 as a halfword, the code uses its low byte. */
typedef struct {
    u8 state;
    u8 pad1;
    u8 counter;
    u8 pad3;
    u8 timer;
    u8 pad5[3];
} Slot04_05Rec;

extern u16 data_801c1630_slot04_05[];
extern int data_801c1700_slot04_05;

void func_801b3a70_slot04_05(Object *obj, int a1, int a2) {
    u8 row = a2;
    Slot04_05Rec *rec = (Slot04_05Rec *)obj->slots + (u8)a1;

    if (obj->field_134 & data_801c1630_slot04_05[row * 3 + 6]) {
        rec->counter--;
        if (rec->counter != 0) {
            rec->timer = 0xc;
        } else {
            data_801c1700_slot04_05 = -1;
        }
    } else {
        rec->timer--;
        if (rec->timer == 0) {
            rec->state = 0;
        }
    }
}
