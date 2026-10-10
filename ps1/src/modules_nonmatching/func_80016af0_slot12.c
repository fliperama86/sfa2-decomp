/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 308 bytes against the original's 352, with another
 * register assignment and instruction order. The exact owner of the bytes
 * in the PS1 build stays the raw bytes of the module image; the build does
 * not use this file. The differential test next to it (difftest.py, with
 * func_80016af0_slot12.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): fills in two adjacent
 * sprite records (0x20 bytes each) for an object. The index idx (low 16
 * bits) selects a list of halfwords: pointer table data_800287ec_slot12
 * entry idx, or, for idx 2, entry object->field_48 of
 * data_8002891c_slot12. The list gives a texture coordinate pair (its
 * first two halfwords, low bytes), and, from its seventh and eighth
 * halfwords, two signed values that go to a library call. Both records get
 * the same coordinates, the position (object field_12 plus the halfword
 * data_80028a10_slot12[idx], and object field_16), texture page 0x7f07 and
 * a command word that is the sum of 0xe1000000, the byte at 0xa3
 * of the block that data_801987c8 points at shifted left by 10, the byte
 * at 0xa2 of that block shifted left by 9, and the low five bits of the
 * library call's result.
 *
 * Contract:
 *   Arguments: a0 = object (0x394 bytes in the test), a1 = pointer to two
 *     records of 0x20 bytes, a2 = idx (only the low 16 bits are used; the
 *     test sets the upper 16 bits at random). Returns nothing.
 *   Reads: object field_12 and field_16 (halfwords) and field_48 (byte);
 *     the pointer tables and the halfword table named above; the 16 bytes
 *     of the list at the selected pointer; the pointer word data_801987c8
 *     and bytes 0xa2 and 0xa3 behind it.
 *   Writes: in each record: the command word at 4, x and y at 0x14 and
 *     0x16, u and v at 0x18 and 0x19, the texture page at 0x1a.
 *   Valid inputs: idx below 76 and field_48 below 61 (the numbers of
 *     entries the test fills in the first and the second pointer table;
 *     the files do not show the tables' lengths); every list pointer is
 *     halfword aligned.
 *   Callee: func_8015bd0c (Sony's library), replaced by a recorder, 4
 *     arguments (0, 0, the first value as signed halfword, the second as
 *     signed halfword); its result is random per case. The log watches the
 *     two records (16 words) at the call.
 *   Aliasing: object, records, lists and the block of data_801987c8 are
 *     distinct.
 *   Not reached: nothing is excluded; every instruction slot is executed.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_800287ec_slot12[];
extern u16 *data_8002891c_slot12[];
extern u16 data_80028a10_slot12[];

void func_80016af0_slot12(Object *obj, Slot12Prim *prim, int idx) {
    Slot12Prim *next = prim + 1;
    u16 *list;
    u16 u;
    u16 v;
    int r;
    u32 cmd;

    idx = (u16)idx;
    if (idx != 2) {
        list = data_800287ec_slot12[idx];
    } else {
        list = data_8002891c_slot12[obj->field_48];
    }
    u = list[0];
    v = list[1];
    prim->u = u;
    prim->v = v;
    next->u = u;
    next->v = v;
    prim->x = obj->pos_x + data_80028a10_slot12[idx];
    prim->y = obj->pos_y;
    next->x = obj->pos_x + data_80028a10_slot12[idx];
    next->y = obj->pos_y;
    prim->tpage = 0x7f07;
    next->tpage = 0x7f07;
    r = func_8015bd0c(0, 0, (s16)list[6], (s16)list[7]);
    cmd = 0xe1000000 + (((u8 *)data_801987c8)[0xa3] << 10) + (((u8 *)data_801987c8)[0xa2] << 9) + (r & 0x1f);
    prim->cmd = cmd;
    next->cmd = cmd;
}
