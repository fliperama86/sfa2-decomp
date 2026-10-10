/*
 * The port's C for func_8001385c_slot01. It runs in the PC program in place
 * of the C of the unit slot01_3440_r3.c; the PS1 build does not use this
 * file, and the unit keeps the exact C.
 *
 * What is different from the unit's C: the call of func_8011ffdc is written
 * with the argument `obj`. The unit calls it with none.
 *
 * Why: func_8011ffdc reads a0 (`lh v0,0x12(a0)` at 8011ffec). On the console
 * a0 holds at that call (jal at 8001388c) what the earlier call left in it,
 * and that is this function's own first parameter. The earlier call is
 * through the table data_80015a64_slot01 of the module slot01 (jalr v0 at
 * 80013884), indexed by the u8 field_05. The table has three entries (the
 * word after them, at 80015a70, is not a code address of the module; inferred:
 * the table ends there). Each entry was read to its return in the listing:
 *   800138a4: one return, `jr ra` at 80013900; it writes a1, v0, v1 and a2,
 *     never a0;
 *   80013908: one return, `jr ra` at 80013984; it calls the leaf at 80013a24
 *     (`jr ra`, a0 never written, jal at 80013974); it writes a2, v0, v1 and
 *     a1 (and ra and sp, restored), never a0;
 *   8001398c: one return, `jr ra` at 800139b4; it writes v0 and v1, never a0.
 * The function itself writes no a0 between its entry and the jal at 8001388c
 * (it writes sp, ra, v0 and at). So a0 is the parameter at the call. Compiled
 * for a PC, the callee would read something else.
 *
 * What the function does (inferred): it calls the entry of the table that
 * field_05 selects with the object, then func_8011ffdc with the object.
 *
 * Contract:
 *   Argument: obj (a0), a pointer to an object; no result.
 *   Reads: the u8 field_05 (index; the setup keeps it 0 to 2). The entries
 *     read and write fields of the object (offsets 0x04, 0x05, 0x12, 0x16,
 *     0x3c) and read the halfwords at 0x801901dc (entry 0) and 0x801901da
 *     (entry 1), the pointer at 0x3c of the object and the halfword at 4
 *     behind it (entry 2).
 *   Callees: the table and its three entries are the image's own and run as
 *     the original code in the test, so that what they leave in a0 is what
 *     the console's code leaves. func_8011ffdc (1 argument) is replaced in
 *     the test by a recorder that logs its argument and returns 0. What it
 *     does is outside the test. The test watches the whole object at the
 *     recorder's call.
 *   Excluded inputs: a null obj; a field_05 of 3 or more (the original would
 *     read past the table).
 *   Not reached by any input: none known (see the coverage line).
 */
#include "../../ps1/src/game.h"
#include "../../ps1/src/protos.h"
#include "../../ps1/src/externs.h"

extern void (*data_80015a64_slot01[])(Object *obj);

void func_8001385c_slot01(Object *obj) {
    data_80015a64_slot01[obj->field_05](obj);
    ((void (*)(Object *))func_8011ffdc)(obj);
}
