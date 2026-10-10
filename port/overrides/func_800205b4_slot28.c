/*
 * The port's C for func_800205b4_slot28. It runs in the PC program in place
 * of the C of the unit slot28_05b4x_r1.c; the PS1 build does not use this
 * file, and the unit keeps the exact C.
 *
 * What is different from the unit's C: the call of func_80131094 is written
 * with the argument `obj`. The unit calls it with none.
 *
 * Why: func_80131094 reads a0 (its first instruction, `move a1,a0` at
 * 80131094, and everything after it goes through a1). On the console a0
 * holds at that call what the earlier call left in it, and that is this
 * function's own first parameter. The earlier call is
 * func_80020608_slot28(obj), read to its end in the listing of the module
 * slot28: it is a leaf with one return, `jr ra` at 80020644 (the delay slot
 * at 80020648 is a store), and it writes no a0 on any path (the instructions
 * from 80020608 to 80020648 write v0, a1, v1 and a2 only). Between that call
 * and the jal at 800205ec the function writes v0 only (800205c8 to 800205e8;
 * the object is kept in s0, set in the delay slot at 800205c4, which does not
 * write a0). So a0 is the parameter at the call. Compiled for a PC, the
 * callee would read something else.
 *
 * What the function does (inferred): it runs func_80020608_slot28 on the
 * object, raises the u8 field_04 by one when the halfword pos_y (offset 0x16)
 * is at least 0xd0, then calls func_80131094 with the object.
 *
 * Contract:
 *   Argument: obj (a0), a pointer to an object; no result.
 *   Reads and writes: the field_04 byte and the pos_y halfword itself; the
 *     earlier callee reads and writes the words of the object at offsets 0x10,
 *     0x14, 0x4c, 0x50, 0x54 and 0x58 (as original code, see below).
 *   Callees: func_80020608_slot28 runs as the original code of the module
 *     image slot28 in the test (a leaf; the test keeps no copy of it). That is
 *     so that what it leaves in a0 is what the console's code leaves.
 *     func_80131094 (1 argument) is replaced in the test by a recorder that
 *     logs its argument and returns 0. What it does is outside the test. The
 *     test watches the whole object at the recorder's call.
 *   Excluded inputs: a null obj (the callees read and write through it).
 *   Not reached by any input: none known (see the coverage line).
 */
#include "../../ps1/src/game.h"
#include "../../ps1/src/protos.h"
#include "../../ps1/src/externs.h"

void func_80020608_slot28(Object *o);

void func_800205b4_slot28(Object *obj) {
    func_80020608_slot28(obj);
    if (obj->pos_y >= 0xd0) {
        obj->field_04++;
    }
    ((void (*)(Object *))func_80131094)(obj);
}
