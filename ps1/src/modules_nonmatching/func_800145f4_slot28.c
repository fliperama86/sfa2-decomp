/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the module image; the build does not use this file.
 * The differential test next to it (difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): when the object's field_f0
 * is 0, it sets the HUD state's field_60 to 600 and adds 1 to its field_52,
 * calls func_80014854_slot28 with the object and 4, clears field_01 of three
 * objects held in a table of the module, and calls func_80128370.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's field_f0; the HUD state pointer data_8018f5a0 and
 *     its field_52; the three object pointers of data_8005187c_slot28.
 *   Writes (only when field_f0 is 0): HUD state field_60 and field_52, and
 *     field_01 of the three table objects.
 *   Aliasing: the object, the HUD state and the three table objects are
 *     distinct blocks.
 *   Callees replaced by recorders (same in both runs): func_80014854_slot28
 *     (two arguments, result 0; it reaches Sony's library) and
 *     func_80128370 (no argument; it waits in a loop for the hardware).
 *     The log copies, at every call, the whole object (0x394 bytes), the
 *     whole HUD state (0x64 bytes) and the three table objects (16 bytes
 *     each), so that a store moved across a call is seen.
 *   Excluded inputs: none.
 *   Not reached by any input: none.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;

/* The table of three objects, inferred from the code. */
extern Object *data_8005187c_slot28[];
void func_80128370(void);
void func_80014854_slot28(Object *obj, int arg);

void func_800145f4_slot28(Object *obj) {
    HudState *hud;

    if (obj->field_f0 == 0) {
        hud = data_8018f5a0;
        hud->field_60 = 600;
        hud->field_52++;
        func_80014854_slot28(obj, 4);
        data_8005187c_slot28[0]->field_01 = 0;
        data_8005187c_slot28[1]->field_01 = 0;
        data_8005187c_slot28[2]->field_01 = 0;
        func_80128370();
    }
}
