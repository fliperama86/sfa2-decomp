/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in one register choice. The
 * exact owner of the bytes in the PS1 build stays the raw bytes of the
 * resident executable; the build does not use this file. The differential
 * test next to it (func_8014c9f4.py, run by difftest.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): starts the object's script
 * (an animation or command stream) when it has not started yet, runs the
 * first step, then reads the 16-bit word that the step left at the script
 * pointer: the high byte goes to field_20e, the low byte to field_20f, and
 * the script pointer advances past the word.
 *
 * Contract:
 *   Argument: a0 = object. No return value.
 *   Reads: object->field_20a, object->field_22c (a pointer to a 16-bit
 *     word, left there by func_8014da18) and that word.
 *   Writes: object->field_20a (set to 1), field_238 (the pointer read from
 *     field_22c), field_20e and field_20f, and data_80189460, which ends as
 *     that pointer plus 2. (The original also stores the pointer itself in
 *     data_80189460 first; the second store replaces it and nothing reads
 *     it in between, so this C stores once.)
 *   Callees replaced by recorders, both taking the object as their one
 *     argument and returning nothing the function uses: func_8014d9ac
 *     (called only when field_20a is 0) and func_8014da18 (always). Their
 *     own effects (the scratchpad tables, the random-number call) are
 *     outside this test; so field_22c is set by the setup.
 *   Watched at every recorded call: the whole object and data_80189460.
 *   Aliasing: the object and the script word are distinct blocks.
 *   Exclusions: none. All instruction slots are reachable.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8014c9f4(Object *object) {
    u16 *script;
    int word;

    if (object->field_20a == 0) {
        func_8014d9ac(object);
    }
    object->field_20a = 1;
    func_8014da18(object);
    script = (u16 *)object->field_22c;
    object->field_238 = (s32)script;
    data_80189460 = script + 1;
    word = *script;
    object->field_20e = word >> 8;
    object->field_20f = word;
}
