/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register allocation and
 * the order of some loads. The exact owner of the bytes in the PS1 build
 * stays the raw bytes of the module image; the build does not use this
 * file. The differential test next to it (func_801b34b8_slot04_17.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not an original name): the update of a thrown
 * or knocked-back character. It moves the object by field_4c (the sign
 * follows field_0b) into the word at field_10, accelerates field_4c by
 * field_54 and stops both when field_4c would go negative. The low byte
 * of field_3a is a pending event code. With no code it calls
 * func_80130efc. With bit 0x80 set it hits the other object (field_40):
 * it advances field_07, counts a hit in the other's field_6a, spawns an
 * effect (func_801465b0), marks the other's field_15b, starts a reaction
 * (func_80140770) with parameters from a byte table indexed by field_12a,
 * and plays one of two sounds (func_80120554) depending on the sign of
 * the other's field_5c, calling func_801b3784_slot04_17 in the negative
 * case; it returns without func_80130efc. With a smaller code it consumes
 * the code (field_3a shifts down a byte), counts the hit, spawns the
 * effect and the reaction check (func_80140cd8) with parameters from a
 * table of halfwords indexed by (code - 8) / 2, picks the same two
 * sounds by that check's result, then calls func_80130efc.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's field_07, field_0b, field_10 (word), field_3a,
 *     field_40 (other object), field_4c, field_54, field_12a; the other
 *     object's field_5c (s16) and field_a6 (side); the byte table
 *     data_801ce44c_slot04_17 at field_12a and field_12a + 1, and the
 *     halfword table data_801ce454_slot04_17 at (code - 8) / 2 .. + 3
 *     (arithmetic shift, so a code of 1 to 7 reads below the table).
 *   Writes: field_07, field_10 (word), field_3a, field_4c, field_54;
 *     ref_other (set to field_40, only on the path with a code below 0x80);
 *     the other object's field_6a, field_15b.
 *   Callees replaced by recorders (they spawn objects, reach the sound
 *     library or have a large state of their own): func_801465b0 (4
 *     arguments), func_80140770 (7, the last three on the stack),
 *     func_80120554 (3), func_80140cd8 (3, result random per case, zero
 *     in half of them), func_801b3784_slot04_17 (1), func_80130efc (1).
 *     Their results are used only for func_80140cd8.
 *   Watched at every recorded call: the whole object (0x400 bytes), the whole
 *     other object, ref_other. No recorded callee
 *     gets a pointer to memory filled for the call.
 *   Aliasing: the object and the other object are distinct blocks.
 *   Excluded inputs: none. field_12a and the code may be any value; the
 *     tables are read inside mapped RAM.
 *   Slots no input reaches: none known; see the coverage line.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801ce44c_slot04_17[];
extern s16 data_801ce454_slot04_17[];

int func_80140cd8(Object *object, int a, int b);
void func_801b3784_slot04_17(Object *obj);

void func_801b34b8_slot04_17(Object *obj) {
    int delta;
    int x;
    int y;
    int j;
    int i;
    u16 flags;
    int code;

    delta = obj->field_4c;
    if (obj->field_0b == 0) {
        delta = -delta;
    }
    *(s32 *)&obj->field_10 = delta + *(s32 *)&obj->field_10;
    obj->field_4c = obj->field_4c + obj->field_54;
    if ((s32)obj->field_4c < 0) {
        obj->field_4c = 0;
        obj->field_54 = 0;
    }
    flags = obj->field_3a;
    code = flags & 0xff;
    if (code != 0) {
        if (code & 0x80) {
            obj->field_07++;
            obj->other->field_6a++;
            func_801465b0(obj, 6, 4, 0x3c);
            obj->other->field_15b = 1;
            i = obj->field_12a;
            func_80140770(obj, 0x10, 5, data_801ce44c_slot04_17[i], data_801ce44c_slot04_17[i + 1], 0, 0);
            if ((s16)obj->other->field_5c < 0) {
                func_80120554(obj->other, obj->other->side, 0x339);
                func_801b3784_slot04_17(obj);
            } else {
                func_80120554(obj->other, obj->other->side, 0x306);
            }
            return;
        }
        code -= 8;
        obj->field_3a = flags >> 8;
        ref_other.p = obj->other;
        ref_other.p->field_6a++;
        j = code >> 1;
        y = data_801ce454_slot04_17[j + 3];
        x = data_801ce454_slot04_17[j + 2];
        func_801465b0(obj, 6, data_801ce454_slot04_17[j], data_801ce454_slot04_17[j + 1]);
        if (func_80140cd8(obj, x, y) != 0) {
            func_80120554(obj->other, obj->other->side, 0x339);
            func_801b3784_slot04_17(obj);
        } else {
            func_80120554(obj->other, obj->other->side, 0x306);
        }
    }
    func_80130efc(obj);
}
