/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the module image; the build does not use this file.
 * The differential test next to it (difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): resets state at the start
 * of a scene part. It clears three halfwords of the HUD state, adds 1 to a
 * fourth, clears a byte of the object (offset 0x4f) and field_64, clears a
 * 16-byte table of the module, sets the HUD state's field_5e to 0xf0, calls
 * func_8011eae4 and func_80013c38_slot28, and sets the object's field_ad
 * to 0xff.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the HUD state pointer data_8018f5a0 and its field_4e.
 *   Writes: HUD state field_50, field_52, field_54 (to 0), field_4e (plus 1),
 *     field_5e (0xf0); the object's byte at offset 0x4f (0), field_64 (0),
 *     field_ad (0xff); the 16 bytes of data_80051ea8_slot28 (0).
 *   Aliasing: the object, the HUD state and the table are distinct blocks.
 *   Callees replaced by recorders (same in both runs): func_8011eae4 (no
 *     argument; the original also has the object in a0 at that call, which
 *     the callee does not read) and func_80013c38_slot28 (one argument, the
 *     object; it reaches the library). Both return 0. The log copies, at
 *     every call, the whole object (0x394 bytes), the whole HUD state
 *     (0x64 bytes) and the table (16 bytes), so that a store moved across a
 *     call is seen. The 0xad store is after both calls in the original.
 *   Excluded inputs: none.
 *   Not reached by any input: none.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
/* A table of 16 bytes, inferred from the code. */
extern u8 data_80051ea8_slot28[16];
void func_80013c38_slot28(Object *obj);

void func_8001389c_slot28(Object *obj) {
    int i;

    data_8018f5a0->field_50 = 0;
    data_8018f5a0->field_52 = 0;
    data_8018f5a0->field_54 = 0;
    data_8018f5a0->field_4e++;
    /* The byte at 0x4f is the last byte of field_4c. */
    ((u8 *)&obj->field_4c)[3] = 0;
    obj->field_64 = 0;
    for (i = 15; i >= 0; i--) {
        data_80051ea8_slot28[i] = 0;
    }
    data_8018f5a0->field_5e = 0xf0;
    func_8011eae4();
    func_80013c38_slot28(obj);
    obj->field_ad = 0xff;
}
