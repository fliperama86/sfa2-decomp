/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 8 bytes smaller (200 against 208) and differs in which
 * registers hold two locals and in the layout of the branches. The exact
 * owner of the bytes in the PS1 build stays the raw bytes of the resident
 * executable; the build does not use this file. The differential test next
 * to it (func_801427d8.py, run by difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): picks a level 0, 2 or 4 as
 * the smaller of two caps, one from the game mode and one from the object's
 * field_c6, stores it in field_12a and field_255, and calls func_80142c04.
 * The mode cap is 4 for the modes 1, 2, 0x94 and 0x68, 2 for 0x90, 0x84,
 * 0x14, 0x60, 0x48 and 0x28, and 0 for every other mode. The field_c6 cap
 * is 0 below 0x60, 2 from 0x60 and 4 from 0x90 (signed compare).
 *
 * Contract:
 *   Argument: a0 = object. No return value.
 *   Reads: game_state.field_354 (the mode, 16 bits), object->field_c6.
 *   Writes: object->field_12a and field_255 (the level); then everything
 *     func_80142c04 writes. That callee runs as the original code. It reads
 *     object->field_7e, field_66 and object->field_40 (a pointer to another
 *     object, which the setup provides) and writes object fields 0x49,
 *     0x29a and 0x29c, the global ref_other (set to field_40), and byte 0x288
 *     or 0x289 of the other object (all read from the original's listing,
 *     not tested).
 *   Aliasing: the object and its field_40 target are distinct blocks.
 *   Exclusions: none. All instruction slots are reachable (inferred).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142c04(Object *object);

void func_801427d8(Object *object) {
    u16 mode = game_state.field_354;
    int limit = 0;
    int level = 0;
    s16 c6;

    if (mode == 1 || mode == 2 || mode == 0x94 || mode == 0x68) {
        limit = 4;
    } else if (mode == 0x90 || mode == 0x84 || mode == 0x14 || mode == 0x60 ||
               mode == 0x48 || mode == 0x28) {
        limit = 2;
    }
    c6 = object->field_c6;
    if (c6 >= 0x60) {
        level = 2;
        if (c6 >= 0x90) {
            level = 4;
        }
    }
    if (limit > level) {
        limit = level;
    }
    object->field_12a = limit;
    object->field_255 = limit;
    func_80142c04(object);
}
