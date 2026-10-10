/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 176 bytes against the original's 236: the original reads
 * the other object's fields again for each of the two stores of a pair,
 * this C reads them once (inferred, not tested). The exact owner of
 * the bytes in the PS1 build stays the raw bytes of the module image; the
 * build does not use this file. The differential test next to it
 * (difftest.py) compares the behavior of this C with the original code on
 * random inputs of the contract below.
 *
 * What it does (inferred, not an original name): copies a strip of 0x30
 * halfwords from a table in RAM into two rows of the table data_801a27e4_rows
 * (rows 0 and 5), then calls func_80137220 with two zero arguments. The
 * strip is chosen by the object's other object: its field_d4 and kind pick
 * the source (a 0x20-byte step per 0x10 halfwords), its side picks where in
 * the row the strip lands.
 *
 * Contract:
 *   Argument: a0 = pointer to an object; its `other` is a second object
 *     (a block distinct from the first and from every table). No return
 *     value.
 *   Reads: other.field_d4, other.kind, other.side; 0x30 halfwords of the
 *     table at data_800e2000 starting at halfword
 *     (field_d4 * 0x3f + kind * 3) * 0x10.
 *   Writes: halfwords 0x20 + side * 0x30 + i, i in 0 to 0x2f, of rows 0 and
 *     5 of data_801a27e4_rows (rows of 0x200 halfwords).
 *   Callee replaced by a recorder (same in both runs): func_80137220 (2
 *     arguments, no result used). The log watches all six rows of
 *     data_801a27e4_rows, whole, at the call.
 *   Aliasing: the objects, the source table and the destination rows are
 *     distinct memory; no source word lies in a destination row.
 *   Excluded inputs: a side above 9. The original does not check it; the
 *     strip written into row 5 would then pass the end of the table
 *     data_801a27e4_rows (side 9 ends at its last halfword). An input that
 *     leaves the table is not part of the contract; the setup keeps side
 *     0 to 9, with 9 often.
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_800e2000[];

void func_8001188c_slot27(Object *obj) {
    Object *other = obj->other;
    int i;
    int first = (other->field_d4 * 0x3f + other->kind * 3) * 0x10;
    int dest = 0x20 + other->side * 0x30;

    for (i = 0; i < 0x30; i++) {
        data_801a27e4_rows[0][dest + i] = data_800e2000[first + i];
        data_801a27e4_rows[5][dest + i] = data_800e2000[first + i];
    }
    func_80137220(0, 0);
}
