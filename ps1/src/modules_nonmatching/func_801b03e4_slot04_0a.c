/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size, instruction
 * scheduling and register choice. The exact owner of the bytes in the PS1
 * build stays the raw bytes of the module image; the build does not use
 * this file. The differential test next to it (difftest.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): a state of a character
 * object that picks a random variant. When the game is not paused (the
 * game_state fields 0x64 and 0x5c are both 0) it sets the object's field_46
 * to 0x3c00, adds one to its field_06, sets game_state.field_76 to 0x1e,
 * draws a number from the random generator, divides it by 52, passes it to
 * func_80125734 together with the object, stores the (signed byte) result
 * in the object's field_48 and starts the sequence numbered 0x23 plus that
 * result with func_80130678. Otherwise it only calls func_80130efc.
 *
 * Contract (what the code reads and writes):
 *   Argument: a0 = pointer to an object (0x394 bytes). No return value.
 *   Reads: game_state.field_64 and field_5c (bytes), the object's field_06;
 *     the random generator's seed (the halfword data_80190126, read and
 *     written by func_80151184) and,
 *     through func_80125734, the object's fields 0xa7, 0xcd and 0x130 and
 *     the tables that function reads.
 *   Writes: the object's field_46, field_06 and field_48,
 *     game_state.field_76, and the seed data_80190126.
 *   Callees run as the original: func_80151184 (random generator) and
 *     func_80125734 (reads the object and constant tables, writes nothing).
 *   Callees replaced by recorders: func_80130678 (2 arguments, the second
 *     under a 16-bit mask because the original passes it sign-extended from
 *     16 bits, read from the original's listing, not tested) and
 *     func_80130efc (1 argument), both returning 0. Their real code steps
 *     the object's animation through the scratchpad and sequence data (read
 *     from the original's listing, not tested), which would need a
 *     contract of its own. The log watches the
 *     whole object (0x394 bytes) and game_state.field_76, so that a field
 *     written after a call instead of before it is a difference.
 *   Aliasing: the object is a block of its own; nothing else is written.
 *   Excluded inputs: none.
 *   Not reached by any input: none expected (see the coverage line).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b03e4_slot04_0a(Object *obj) {
    s8 variant;

    if (game_state.field_64 == 0 && game_state.field_5c == 0) {
        obj->field_46 = 0x3c00;
        obj->field_06 = obj->field_06 + 1;
        game_state.field_76 = 0x1e;
        variant = func_80125734(obj, (s8)(func_80151184() / 52));
        obj->field_48 = variant;
        func_80130678(obj, variant + 0x23);
    } else {
        func_80130efc(obj);
    }
}
