/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code addresses the final store through a saved register where the
 * original uses the argument register. The exact owner of the bytes in the
 * PS1 build stays the raw bytes of the original; the build does not use this
 * file. The differential test next to it (func_80137e38.py, run with
 * difftest.py) compares this C with the original code on random inputs of
 * the contract below.
 *
 * What it does (inferred, not an original name): a state-change step for an
 * object. It sets the object's flag byte field_80 to 1, advances its counter
 * field_06, toggles bit 0 of field_0b, calls func_801380f0, then sets bit 9
 * (0x200) of the halfword field_46 (the low byte of the old value is kept,
 * its high byte is dropped) and calls func_80131094.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads and writes the object's field_06, field_0b, field_46, field_80.
 *   Callees: func_801380f0 and func_80131094 are replaced by recorders that
 *     log their one argument (the object) and return 0; what they do
 *     themselves is outside the test. The log shows the calls were made, in
 *     this order, with this argument. The whole object (0x394 bytes) is
 *     watched: each recorder copies it at the call, so the stores made
 *     before and after each call are compared in order. No callee gets a
 *     pointer to memory filled for the call.
 *   Aliasing: the object is one block; nothing else is read or written.
 *   Excluded inputs: none. Every instruction slot is executed.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80137e38(Object *object) {
    object->field_80 = 1;
    object->field_06++;
    object->field_0b ^= 1;
    func_801380f0(object);
    object->field_46 = (u8)object->field_46 | 0x200;
    func_80131094(object);
}
