/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * original loads the base pointer first and keeps the position and the
 * header word in other registers than the code built from this C. The exact
 * owner of the bytes in the PS1 build stays the raw bytes of the resident
 * image; the build does not use this file. The differential test next to it
 * (difftest.py) compares the behavior of this C with the original code on
 * random inputs of the contract below.
 *
 * What it does (inferred, not an original name): makes a record the
 * object's current sequence, copies the record's header word into the
 * object (high half to field_38, low half to field_3a), then copies the
 * record's data block into a grid of cells at a place derived from the
 * object's position: pos_x selects a column (bits 9 to 11 and bits 5 to 8
 * of pos_x) and pos_y / 32 a row, in a buffer whose address is in cam_f50.
 *
 * Contract:
 *   Arguments: a0 = the object, a1 = the record (header word at 0, pointer
 *     to a data block at 4).
 *   No return value.
 *   Reads: the object's pos_x (as unsigned 16 bits) and pos_y; the record's
 *     two words; the pointer cam_f50.
 *   Writes: the object's sequence (0x18), field_38 and field_3a; through
 *     func_8011fb70 the destination cells (see below).
 *   Callee func_8011fb70 (game code, copies a grid of 16-bit cells from the
 *     data block, whose first two halfwords are the columns and rows count)
 *     runs as the original in both runs (what it copies is read from its
 *     listing, not tested); the destination is the buffer in
 *     cam_f50 plus a signed offset of 17 bits. The setup keeps the data
 *     block, the object and the buffer in separate blocks, and counts of
 *     0 to 6.
 *   Not reached by any input (read from the listing): one instruction
 *     slot of the original, at offset 0x40, which adds 31 to the masked
 *     pos_x bits before dividing by 32; the masked value (pos_x & 0x1e0) is never negative.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declaration; not an original one. */
extern u8 *cam_f50;

void func_8011fa50(Object *object, SeqRec *rec) {
    u32 x;
    int offset;
    int cell_y;
    int col;

    object->sequence = (SequenceStep *)rec;
    object->field_38 = rec->header >> 16;
    object->field_3a = rec->header;
    x = (u16)object->pos_x;
    col = ((x & 0x1e0) >> 5) * 4;
    cell_y = ((object->pos_y / 32) << 22) >> 16;
    offset = (x & 0xe00) + col + cell_y;
    func_8011fb70(rec->data, (u16 *)(cam_f50 + ((offset << 15) >> 15)));
}
