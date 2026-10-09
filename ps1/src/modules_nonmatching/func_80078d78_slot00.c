/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in the registers that hold
 * the constant 1 and the saved pos_y (two registers exchanged). The exact
 * owner of the bytes in the PS1 build stays the raw bytes of the module
 * image; the build does not use this file. The differential test next to it
 * (func_80078d78_slot00.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): on every eighth frame
 * (low three bits of game_state.field_1d clear) it takes a free object
 * from the pool (func_8011f1e0) and, when it got one, fills it in as a
 * child of the object it was given: kind fields 1, 0xb, 0x20 and 2, the
 * parent's field_0e, field_0b, position and field_66, the parent pointer in
 * field_3c, a pair of size words (0x60, 0x1e0), a flag, and three data
 * pointers.
 *
 * Contract:
 *   Argument: a0 = pointer to an object (the parent). No return value.
 *   Reads: game_state.field_1d; the parent's field_0e, field_0b, pos_x,
 *     pos_y and field_66.
 *   Writes (only when the pool gives an object, the new object): field_00,
 *     field_02, field_03, field_08, field_0e, field_3c, field_0b, pos_x,
 *     pos_y, field_7a, field_7c, field_44, field_90, field_98, field_9c,
 *     field_66.
 *   Callee replaced by a recorder: func_8011f1e0 (no arguments; the pool
 *     allocator, which reaches the pool tables). It returns the setup's new
 *     object, or 0 in a quarter of the cases. The log watches the new
 *     object, whole (0x394 bytes), and the parent.
 *   Aliasing: the parent and the new object are distinct blocks.
 *   Excluded inputs: none.
 *   Slots no input reaches: none known; see the coverage line.
 * The tree declares func_8011f1e0 as returning a Block172 pointer; the result is cast to Object * here.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"


void func_80078d78_slot00(Object *obj) {
    Object *child;

    if ((game_state.field_1d & 7) == 0) {
        child = (Object *)func_8011f1e0();
        if (child != 0) {
            child->field_00 = 1;
            child->field_02 = 0xb;
            child->field_08 = 0x20;
            child->field_03 = 2;
            child->field_0e = obj->field_0e;
            child->field_3c = obj;
            child->field_0b = obj->field_0b;
            child->pos_x = obj->pos_x;
            child->field_7a = 0x60;
            child->field_7c = 0x1e0;
            child->field_44 = 1;
            child->field_90 = &data_800fb100;
            child->field_98 = data_80172a48;
            child->field_9c = data_80173c9c;
            child->pos_y = obj->pos_y;
            child->field_66 = obj->field_66;
        }
    }
}
