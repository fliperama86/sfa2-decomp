/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is shorter (the original reloads the address of ref_other for
 * each use, repeats the advance block as two paths and widens the argument
 * of func_80140cd8 to 16 bits by shifts). The PS1 build keeps the raw bytes of the module image and does
 * not use this file. The differential test next to it (difftest.py, with
 * func_801b37b0_slot04_03.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a state of a character that
 * is struck or held by the other object. It stores the other object
 * (field_40) in ref_other. When the low byte of field_3a is not 0 it clears
 * that byte, makes a sound with func_801204f4, calls func_80146478 and
 * func_80140cd8 (with 8 the first time field_1cf is 0, which then becomes
 * 0xff, otherwise 0). If func_80140cd8 returns a nonzero byte it calls
 * func_80140770, sounds 0x340 for the other object and goes to the advance.
 * Otherwise it adds 1 to field_46 and sounds 0x30b. Then (and when the low
 * byte of field_3a was 0) it calls func_80140fe0; if field_46 is 0 it
 * hands over to func_80130efc; otherwise it reloads ref_other from field_40
 * and advances when the other object's field_15b is 0 or when
 * func_801410c8 returns a nonzero byte, and hands over to func_80130efc if
 * not. The advance adds 1 to field_07, sounds 0x30d for the other object,
 * makes the sound for the object and starts sequence 0x41.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: field_40, field_3a, side, field_1cf, field_46, field_07 and, of
 *     the other object, side and field_15b.
 *   Writes: ref_other (the other object's address), field_3a (low byte
 *     cleared), field_1cf (0xff), field_46 (+1), field_07 (+1), and what the
 *     callees write.
 *   Callees, all replaced by recorders in the original and in this C alike
 *     (they reach the sound, sprite and sequence code of the resident
 *     image): func_801204f4 (3 arguments), func_80120554 (3), func_80146478
 *     (4), func_80140cd8 (3; the result is a random word, only its low byte is
 *     used), func_80140770 (7), func_80140fe0 (1), func_801410c8 (1; random
 *     word, low byte used), func_801307e0 (2), func_80130efc (1).
 *     No recorded callee gets a pointer to memory filled for the call.
 *   Aliasing: the object and the other object are distinct blocks.
 *   Watched by the recorders: the whole object, the whole other object (0x394
 *     bytes each) and the word ref_other, at every call, so the order of this
 *     function's stores against the calls is tested.
 *   Inputs excluded: none. Slots not reached: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Declared here because the published headers lack them (inferred). */
int func_80140cd8(Object *object, int a, int b);
void func_80146478(Object *object, u8 a, int dx, int dy);
void func_80140fe0(Object *object);
int func_801410c8(Object *object);

static void advance(Object *obj) {
    obj->field_07++;
    func_80120554(ref_other.p, ref_other.p->side, 0x30d);
    func_801204f4(obj, obj->side, 7);
    func_801307e0(obj, 0x41);
}

void func_801b37b0_slot04_03(Object *obj) {
    int arg;

    ref_other.p = obj->other;
    if ((obj->field_3a & 0xff) != 0) {
        obj->field_3a &= 0xff00;
        func_801204f4(obj, obj->side, 7);
        func_80146478(obj, 1, -0x2d, 0x2b);
        arg = 0;
        if (((Slot04aObj *)obj)->field_1cf == 0) {
            ((Slot04aObj *)obj)->field_1cf = 0xff;
            arg = 8;
        }
        if ((func_80140cd8(obj, arg, 0) & 0xff) != 0) {
            func_80140770(obj, 0, 5, 0, 0, 0, 1);
            func_80120554(ref_other.p, ref_other.p->side, 0x340);
            advance(obj);
            return;
        }
        obj->field_46++;
        func_80120554(ref_other.p, ref_other.p->side, 0x30b);
    }
    func_80140fe0(obj);
    if ((s16)obj->field_46 == 0) {
        func_80130efc(obj);
        return;
    }
    ref_other.p = obj->other;
    if (ref_other.p->field_15b == 0 || (func_801410c8(obj) & 0xff) != 0) {
        advance(obj);
    } else {
        func_80130efc(obj);
    }
}
