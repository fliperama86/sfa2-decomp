/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes (156 bytes against 160:
 * the half is kept in one register, where the original keeps two copies).
 * The exact owner of the bytes in the PS1 build stays the
 * raw bytes of the module image; the build does not use this file. The
 * differential test next to it (func_80077868_slot2b.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): sets field_07 to 3 and
 * field_159 to 1. When field_219 is not 0 it calls func_80077908_slot2b
 * and ends. Otherwise it takes half of field_12a, passes it to
 * func_80141f28 (which adds a signed amount to a gauge of the object), and
 * then calls func_801307e0 with that half plus a base, 0xc when field_48
 * is 0 and 0x12 otherwise, plus 3 when field_129 is not 0, truncated to 16
 * bits.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's field_219, field_12a, field_48, field_129.
 *   Writes: field_07 and field_159.
 *   Callees replaced by recorders returning 0 (their results are not used;
 *     func_80141f28 reaches the library's sound, func_801307e0 the sequence
 *     start, func_80077908_slot2b is the next function of the module):
 *     func_80077908_slot2b (1 argument: the object), func_80141f28 (2: the
 *     object and the half), func_801307e0 (2: the object and the sum). The
 *     log watches the object, whole (0x394 bytes), at every call.
 *   Aliasing: none (one object).
 *   Excluded inputs: none.
 *   Slots no input reaches: none known; see the coverage line.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void func_80077908_slot2b(Object *obj);

void func_80077868_slot2b(Object *obj) {
    int half;
    int step;

    obj->field_07 = 3;
    obj->field_159 = 1;
    if (obj->field_219 != 0) {
        func_80077908_slot2b(obj);
    } else {
        half = obj->field_12a >> 1;
        func_80141f28(obj, half);
        step = obj->field_48 == 0 ? 0xc : 0x12;
        if (obj->field_129 != 0) {
            step += 3;
        }
        func_801307e0(obj, (u16)(half + step));
    }
}
