/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction order and
 * register choice (36 of 72 instruction slots; same size, 288 bytes; the
 * dead first store of field_02 is not made, read from the original's
 * listing, not tested). The exact owner of the bytes in the PS1
 * build stays the raw bytes of the module image; the build does not use
 * this file. The differential test next to it (func_800796e4_slot2b.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not an original name): spawns a child effect of
 * an object. It takes a free object from the pool (func_8011f1e0) and, when
 * it got one, fills it in: kind fields 1, 0x14, 1 and 0, field_44 1,
 * field_48 0x52, the size words 0x60 and 0x1e0, field_08 0x20, the parent
 * pointer of the spawning object (its field_3c) in field_3c, and copies of
 * the spawning object's field_65, field_0d, field_66, field_90, field_98
 * and field_9c. The child's position is the spawning object's position
 * plus an offset pair from the table data_8007ee64_slot2b, selected by a
 * random number (func_80151184) masked to an even index below 32: x adds
 * the first half of the pair, y subtracts the second. Last, and only when
 * it got one, it calls func_801204f4 with the spawning object, the
 * parent's side and 0xf.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's field_3c, field_65, pos_x, pos_y, field_0d,
 *     field_90, field_66, field_98, field_9c; the parent's side (field_a6);
 *     the table data_8007ee64_slot2b (32 halfwords); the random seed
 *     data_80190126 (through func_80151184).
 *   Writes (only when the pool gives an object, the new object): field_00,
 *     field_02, field_03, field_09, field_65, field_44, field_48, field_7a,
 *     field_3c, field_7c, pos_x, pos_y, field_0d, field_08, field_90,
 *     field_66, field_98, field_9c; and the seed data_80190126.
 *   Callees: func_80151184 runs as original code (game code, reads and
 *     writes the seed). Replaced by recorders returning no used value:
 *     func_8011f1e0 (no arguments; the pool allocator; it returns the new
 *     object of the setup, or 0 in a quarter of the cases) and
 *     func_801204f4 (3 arguments, reaches the sequence tables, read from
 *     the original's
 *     listing, not tested). The log
 *     watches the new object, the spawning object and the parent, whole
 *     (0x394 bytes each), at every call.
 *   Aliasing: the object, the new object and the parent are distinct
 *     blocks.
 *   Excluded inputs: none.
 *   Slots no input reaches: none known; see the coverage line. (The
 *     original also stores 0x3b into field_02 and overwrites it with 0x14
 *     before any call, read from the original's listing, not tested; the C
 *     stores only the 0x14, with no difference in
 *     any state a callee or the test can observe.)
 * The tree declares func_8011f1e0 as returning a Block172 pointer; the result is cast to Object * here.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_8007ee64_slot2b[];

void func_800796e4_slot2b(Object *obj) {
    Object *parent = obj->field_3c;
    Object *child = (Object *)func_8011f1e0();
    int index;

    if (child != 0) {
        child->field_00 = 1;
        child->field_03 = 1;
        child->field_09 = 0;
        child->field_02 = 0x14;
        child->field_65 = obj->field_65;
        child->field_44 = 1;
        index = func_80151184() & 0x1e;
        child->field_48 = 0x52;
        child->field_7a = 0x60;
        child->field_3c = parent;
        child->field_7c = 0x1e0;
        child->pos_x = obj->pos_x + data_8007ee64_slot2b[index];
        child->pos_y = obj->pos_y - data_8007ee64_slot2b[index + 1];
        child->field_0d = obj->field_0d;
        child->field_08 = 0x20;
        child->field_90 = obj->field_90;
        child->field_66 = obj->field_66;
        child->field_98 = obj->field_98;
        child->field_9c = obj->field_9c;
        func_801204f4(obj, parent->side, 0xf);
    }
}
