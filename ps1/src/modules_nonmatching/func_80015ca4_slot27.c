/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code orders two constant loads (the ori of 0x80038000 and the two
 * small constants) differently from the original. The exact owner of the
 * bytes in the PS1 build stays the raw bytes of the module image; the build
 * does not use this file. The differential test next to it (difftest.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not an original name): the per-frame step of an
 * object that spawns one helper object. With the object's field_60 set it
 * hands the object to the shared helper and then to the object update. With
 * field_60 clear and field_05 set it counts field_04 up when the game state's
 * field_04 is set, then runs the object update. With both clear and the
 * halfword data_8002b386_slot27 negative it sets field_05 to count it, asks
 * for a new object and, when it gets one, fills in a fixed set of fields.
 * Otherwise it calls the module's own step function.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: obj.field_60, obj.field_05, obj.field_04, game_state.field_04,
 *     game_state.field_40 (low byte), data_8002b386_slot27, and the addresses
 *     of data_80017c28_slot27 and data_8001aa14_slot27.
 *   Writes: obj.field_04 (incremented) or obj.field_05 (incremented); on a
 *     new object b (not 0): b.field_00, field_01, field_02, field_03,
 *     field_60, field_90, field_98, field_9c, field_7a, field_7c, field_0d.
 *   Callees replaced by recorders (same in both runs): func_80131094 (1
 *     argument), func_8011ffdc (1), func_80016810_slot27 (1), func_8011f1e0
 *     (no argument; returns 0 or the address of a block the setup made, in a
 *     tenth of the cases 0). Watched at every call: the whole object (0x394
 *     bytes) and the whole new block, so that a field written after a call
 *     instead of before it would show.
 *   Aliasing: the object and the new block are distinct blocks.
 *   Excluded inputs: none.
 *   Not reached by any input: none expected.
 *   The value stored in field_90 is the constant 0x80038000 as the original
 *   loads it; it is a value of the field, whose meaning is not known, and
 *   the tree's other units of this module spell it the same way.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern s16 data_8002b386_slot27;
void func_80016810_slot27(Object *obj);

void func_80015ca4_slot27(Object *obj) {
    Object *b;

    if (obj->field_60 != 0) {
        func_80131094(obj);
        func_8011ffdc(obj);
    } else if (obj->field_05 != 0) {
        if (game_state.field_04 != 0) {
            obj->field_04++;
        }
        func_8011ffdc(obj);
    } else if (data_8002b386_slot27 < 0) {
        obj->field_05++;
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0xaa;
            b->field_01 = 1;
            b->field_60 = 1;
            b->field_03 = *(u8 *)&game_state.field_40;
            b->field_98 = data_80017c28_slot27;
            b->field_9c = data_8001aa14_slot27;
            b->field_90 = (void *)0x80038000;
            b->field_7a = 0;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
        }
    } else {
        func_80016810_slot27(obj);
    }
}
