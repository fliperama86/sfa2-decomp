/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling,
 * register choice and stack frame. The exact owner of the bytes in the PS1
 * build stays the raw bytes of the module image; the build does not use
 * this file. The differential test next to it (difftest.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * The build is 344 bytes, the original 432: the original first clears the
 * low half of record words 0 to 0xc and stores the pointer at 0x18 twice;
 * every one of those stores is overwritten before the function ends, and
 * this C leaves them out.
 *
 * What it does (inferred, not an original name): initializes an object for
 * its first frame. It sets a few flag bytes, counts up field_04, and fills
 * the 0x20-byte record of data_801f3010_slot06_00 selected by the object's
 * field_03 from four consecutive entries of the word table
 * data_801e9f60_slot06_00 (entries 4*n to 4*n+3, n = field_03; the index is
 * taken as a byte): two entries are copied whole, and the other two give a
 * word with the low half cleared and a word that is 0xf80000 minus their
 * low half shifted up by 16. The record also gets pointers into the
 * sequence table data_801e9f48_slot06_00 (the step field_03 and the one
 * after it); the object's sequence is set to the first. Then it sets
 * fixed size fields and mirrors pos_y (0xf8 minus its low 16 bits).
 *
 * Contract (what the code reads and writes; the roles named for the fields
 * are inferred):
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's field_03, field_04 and pos_y; four entries of
 *     data_801e9f60_slot06_00 (word index (4*field_03 + k) & 0xff, k 0 to 3);
 *     the pointer data_801e9f48_slot06_00.
 *   Writes: the object's field_04, field_0a, field_0c, field_0d, field_0f,
 *     sequence, field_76, field_78, field_7a, field_7c, field_81 and pos_y;
 *     all 0x20 bytes of record field_03 of data_801f3010_slot06_00.
 *   Aliasing: the object, the record table, the word table and the
 *     sequence table are distinct blocks. The record index is a byte, so
 *     the record table has 256 records.
 *   Every instruction slot of the original is reached by some input.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801e9f48_slot06_00;
extern u32 data_801e9f60_slot06_00[];
extern Slot06_00Rec3010 data_801f3010_slot06_00[];

void func_801e8dc4_slot06_00(Object *obj) {
    Slot06_00Rec3010 *r;
    u8 i;

    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_0c = 0;
    obj->field_04++;
    r = &data_801f3010_slot06_00[obj->field_03];
    i = obj->field_03 << 2;
    r->field_10 = data_801e9f60_slot06_00[i];
    r->field_00 = data_801e9f60_slot06_00[(u8)(i | 1)] & 0xffff0000;
    r->field_04 = 0xf80000 - (*(u16 *)&data_801e9f60_slot06_00[(u8)(i | 1)] << 16);
    r->field_14 = data_801e9f60_slot06_00[(u8)(i | 2)];
    r->field_08 = data_801e9f60_slot06_00[(u8)(i | 3)] & 0xffff0000;
    r->field_0c = 0xf80000 - (*(u16 *)&data_801e9f60_slot06_00[(u8)(i | 3)] << 16);
    r->field_18 = data_801e9f48_slot06_00 + obj->field_03;
    r->field_1c = r->field_18 + 1;
    obj->sequence = r->field_18;
    obj->field_76 = 0x280;
    obj->field_78 = 0x100;
    obj->field_7a = 0x70;
    obj->field_7c = 0x1e0;
    obj->field_81 = 4;
    obj->field_0d = 0;
    obj->pos_y = 0xf8 - (u16)obj->pos_y;
}
