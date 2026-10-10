/*
 * The port's C for func_8001714c_slot28. It runs in the PC program in place
 * of the C of the unit slot28_714cx_r1.c; the PS1 build does not use this
 * file, and the unit keeps the exact C.
 *
 * What is different from the unit's C: the call of func_80131094 is written
 * with the argument `obj`. The unit calls it with none.
 *
 * Why: func_80131094 reads a0 (its first instruction, `move a1,a0` at
 * 80131094). On the console a0 holds at that call (jal at 80017184) what the
 * earlier call left in it, and that is this function's own first parameter.
 * The earlier call is through the table data_800306cc_slot28 of the module
 * slot28 (jalr v0 at 8001717c), indexed by the u8 field_05. The table has two
 * entries (inferred: the word after them, at 800306d4, is 0 and the table is
 * read as ending there). Each entry was read to its return in the listing:
 *   800171a8 (reads a0 at 800171a8, 800171b8 and 800171c4; raises field_05
 *     when the byte at 0x48 is not 0), one return, `jr ra` at 800171c8; it
 *     writes no register but v0;
 *   800171d0: `jr ra` at once.
 * Neither writes a0 on any path. The function itself writes no a0 between the
 * entry (the object is kept in s0, set at 80017154) and the jal at 80017184.
 * So a0 is the parameter at the call. Compiled for a PC, the callee would
 * read something else.
 *
 * What the function does (inferred): it calls the entry of the table that
 * field_05 selects with the object, then func_80131094 with the object, then
 * sets the u8 field_01 to 1.
 *
 * Contract:
 *   Argument: obj (a0), a pointer to an object; no result.
 *   Reads and writes: the u8 field_05 (index; the setup keeps it 0 or 1) and
 *     field_01 (written). Entry 0 reads the byte at 0x48 and may write
 *     field_05.
 *   Callees: the table and its two entries are the image's own and run as
 *     the original code in the test, so that what they leave in a0 is what
 *     the console's code leaves. func_80131094 (1 argument) is replaced in
 *     the test by a recorder that logs its argument and returns 0. What it
 *     does is outside the test. The test watches the whole object at the
 *     recorder's call.
 *   Excluded inputs: a null obj; a field_05 of 2 or more (the original would
 *     read past the table).
 *   Not reached by any input: none known (see the coverage line).
 */
#include "../../ps1/src/game.h"
#include "../../ps1/src/protos.h"
#include "../../ps1/src/externs.h"

extern void (*data_800306cc_slot28[])(Object *);

void func_8001714c_slot28(Object *obj) {
    data_800306cc_slot28[obj->field_05](obj);
    ((void (*)(Object *))func_80131094)(obj);
    obj->field_01 = 1;
}
