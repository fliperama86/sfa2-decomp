/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register choice for the
 * position sum. The exact owner of the bytes in the PS1 build stays the raw
 * bytes of the module image; the build does not use this file. The
 * differential test next to it (difftest.py, with func_801b4a3c_slot04_0e.py)
 * compares the behavior of this C with the original code on random inputs of
 * the contract below.
 *
 * What it does (inferred, not an original name): a character action handler
 * that, on every eighth value of the frame counter (game_state.field_1d, low
 * three bits zero), asks for a free object slot and, if it gets one, fills
 * it as an effect object that follows this one (field_3c points back at the
 * object, the position is the object's plus the record's x offset). It does
 * the same as func_801b420c_slot04_0e for the slot (with field_48 0x4e
 * instead of 0x4b) but changes nothing in the object itself.
 *
 * Contract:
 *   Argument: a0 = pointer to an object (Object, 0x394 bytes). No return
 *   value.
 *   Reads: game_state.field_1d; the object's side (index into the table of
 *     0x1c-byte records at data_801c62b4_slot04_0e, whose field_10 is the x
 *     offset in its high half), pos_x, field_70, field_0d, field_90,
 *     field_66, field_98, field_9c.
 *   Writes: nothing when field_1d & 7 is not 0 or the slot is 0; otherwise
 *     the slot's field_00, field_02, field_03, field_08, field_09, field_0d,
 *     pos_x, pos_y, field_3c, field_48, field_66, field_7a, field_7c,
 *     field_90, field_98, field_9c. The original writes field_02 twice (0x3b,
 *     then 0x14); only the last value is left, so only it is written here.
 *   Callee, replaced by a recorder: func_8011f1e0 (0 arguments) returns
 *     either 0 or a slot block of the case.
 *   Watched in the test: the whole object and the whole slot block, copied by
 *     every recorder at every call. No recorded callee gets a pointer to
 *     memory filled for the call.
 *   Aliasing: the object, the slot and the record table are distinct.
 *   Excluded inputs: none. Instruction slots no input can reach: none.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot04_0eRec62b4 data_801c62b4_slot04_0e[];

/* Inferred: returns a free object slot, or 0. */
Block172 *func_8011f1e0(void);

void func_801b4a3c_slot04_0e(Object *obj) {
    Slot04_0eRec62b4 *record = &data_801c62b4_slot04_0e[obj->side];
    Object *slot;

    if ((game_state.field_1d & 7) == 0) {
        slot = (Object *)func_8011f1e0();
        if (slot != 0) {
            slot->field_00 = 1;
            slot->field_02 = 0x14;
            slot->field_03 = 1;
            slot->field_09 = 0;
            slot->pos_x = obj->pos_x + (record->field_10 >> 16);
            slot->pos_y = obj->field_70;
            slot->field_48 = 0x4e;
            slot->field_7a = 0x60;
            slot->field_3c = obj;
            slot->field_7c = 0x1e0;
            slot->field_0d = obj->field_0d;
            slot->field_08 = 0x20;
            slot->field_90 = obj->field_90;
            slot->field_66 = obj->field_66;
            slot->field_98 = obj->field_98;
            slot->field_9c = obj->field_9c;
        }
    }
}
