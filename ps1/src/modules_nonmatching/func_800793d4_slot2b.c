/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice (200 bytes against the original's 204). The exact
 * owner of the bytes in the PS1 build stays the raw bytes of the module image; the build does
 * not use this file. The differential test next to it
 * (func_800793d4_slot2b.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): an update step of an
 * object that falls toward a floor line given by another object (the one
 * data_8007ef34_slot2b points at). It first calls func_800795b4_slot2b.
 * When the floor's field_70 is less than the object's pos_y (signed halves)
 * the object has landed: field_05 goes up by one, pos_y is set to the
 * floor's field_70, field_0c, field_0d and field_14 are cleared, the three
 * data pointers field_90, field_98 and field_9c are set, a sequence is
 * started (func_80130700 with data_8017c850) and func_801204f4 is called
 * with the object, the floor object's side and 0x10. Otherwise it calls
 * func_80131094.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's pos_y and field_05; the pointer
 *     data_8007ef34_slot2b; the floor object's field_70 (s16) and side
 *     (field_a6); the pointer data_8017c850.
 *   Writes (landing arm): field_05, field_90, field_98, field_9c, field_14,
 *     field_0c, field_0d and pos_y of the object.
 *   Callees replaced by recorders returning 0 (their results are not
 *     used): func_800795b4_slot2b (1 argument: the object; the next
 *     function of the module, read from the original's listing, not
 *     tested), func_80130700 (2: the object and the sequence pointer),
 *     func_801204f4 (3: the object, side, 0x10; reaches the sequence
 *     tables, read from the original's listing, not tested),
 *     func_80131094 (1: the object). The log watches
 *     the object and the floor object, whole (0x394 bytes each), at every
 *     call. The sequence pointer is passed on without being read through.
 *   Aliasing: the object and the floor object are distinct blocks.
 *   Excluded inputs: none.
 *   Slots no input reaches: none known; see the coverage line.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;
extern SequenceStep *data_8017c850;

extern void func_800795b4_slot2b(Object *obj);

void func_800793d4_slot2b(Object *obj) {
    func_800795b4_slot2b(obj);
    if (data_8007ef34_slot2b->field_70 < obj->pos_y) {
        obj->field_05++;
        obj->field_90 = &data_800fb100;
        obj->field_98 = data_80172a48;
        obj->field_14 = 0;
        obj->field_0c = 0;
        obj->field_0d = 0;
        obj->pos_y = data_8007ef34_slot2b->field_70;
        obj->field_9c = data_80173c9c;
        func_80130700(obj, data_8017c850);
        func_801204f4(obj, data_8007ef34_slot2b->side, 0x10);
    } else {
        func_80131094(obj);
    }
}
