/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in the order of two stores
 * and in register choice for the position sum. The exact owner of the bytes
 * in the PS1 build stays the raw bytes of the module image; the build does
 * not use this file. The differential test next to it (difftest.py, with
 * func_801b4100_slot04_0e.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a character action handler
 * that asks for a free object slot and, if it gets one, fills it as an
 * effect object tied to the object's partner (obj->other): the slot copies
 * the partner's field_0e and field_0b and takes the partner's height as its
 * pos_y, the position x is the object's plus the record's x offset, and
 * field_3c points at the partner; it then calls func_80120554 for the
 * partner. In every case it ends by calling func_801b420c_slot04_0e on the
 * object.
 *
 * Contract:
 *   Argument: a0 = pointer to an object (Object, 0x394 bytes). No return
 *   value.
 *   Reads: the object's side (index into the table of 0x1c-byte records at
 *     data_801c62b4_slot04_0e, whose field_10 is the x offset in its high
 *     half), other, pos_x, field_65, field_0d, field_90, field_98, field_9c;
 *     when a slot is returned also the partner's field_0e, field_0b,
 *     field_70 and side.
 *   Writes: nothing itself when the slot is 0; otherwise the slot's
 *     field_00, field_02, field_03, field_0b, field_0d, field_0e, pos_x,
 *     pos_y, field_3c, field_48, field_65, field_7a, field_7c, field_90,
 *     field_98, field_9c.
 *   Callees, replaced by recorders: func_8011f1e0 (0 arguments) returns
 *     either 0 or a slot block of the case; func_80120554 (3 arguments:
 *     the partner, its side, 0x312) returns 0; func_801b420c_slot04_0e
 *     (1 argument) returns 0. The last is a function of this image with
 *     its own test; recorded here so that this test sees what is passed.
 *   Watched in the test: the whole object, its partner and the whole slot
 *     block, copied by every recorder at every call. No recorded callee gets
 *     a pointer to memory filled for the call.
 *   Aliasing: the object, its partner, the slot and the record table are
 *     distinct blocks.
 *   Excluded inputs: none. Instruction slots no input can reach: none.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot04_0eRec62b4 data_801c62b4_slot04_0e[];

/* Inferred: returns a free object slot, or 0. */
Block172 *func_8011f1e0(void);
void func_801b420c_slot04_0e(Object *obj);

void func_801b4100_slot04_0e(Object *obj) {
    Slot04_0eRec62b4 *record;
    Object *slot;
    Object *other;

    record = &data_801c62b4_slot04_0e[obj->side];
    slot = (Object *)func_8011f1e0();
    if (slot != 0) {
        other = obj->other;
        slot->field_00 = 1;
        slot->field_02 = 4;
        slot->field_03 = 6;
        slot->field_0e = other->field_0e;
        slot->field_0b = other->field_0b;
        slot->pos_x = obj->pos_x + (record->field_10 >> 16);
        slot->field_3c = other;
        slot->pos_y = other->field_70;
        slot->field_48 = 1;
        slot->field_65 = obj->field_65;
        slot->field_7a = 0x60;
        slot->field_7c = 0x1e0;
        slot->field_0d = obj->field_0d;
        slot->field_90 = obj->field_90;
        slot->field_98 = obj->field_98;
        slot->field_9c = obj->field_9c;
        func_80120554(other, other->side, 0x312);
    }
    func_801b420c_slot04_0e(obj);
}
