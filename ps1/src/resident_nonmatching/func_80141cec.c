/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 8 bytes larger (336 against 328) and orders the or-chain of
 * the first test and its registers differently. The exact owner of the bytes
 * in the PS1 build stays the raw bytes of the resident executable; the
 * build does not use this file. The differential test next to it
 * (func_80141cec.py, run by difftest.py) compares the behavior of this C
 * with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): tries to start an action
 * for the object. Unless the object's bytes 4 to 7 hold exactly 1, 1, 2, 0
 * it first checks that the object is idle: field_7e and field_45 are 0,
 * bytes 4 and 5 are 1 and 0, and byte 6 is not 7, 8 or 9; otherwise it
 * returns 0. It clears field_69 and field_6a; when game_state.field_30 is
 * set and game_state.mode is not the object's side plus 1, it clears
 * field_19f and field_1a2. It returns 0 when any of three bytes of the
 * game configuration (fields 4d, 4e, 04) is set, or when func_8012f56c
 * (a test on the partner object) returns non-zero. In the idle-checked
 * case (the bytes were not 1, 1, 2, 0) it also sets field_bd to 2 and,
 * when field_cd is 0, calls func_80155d4c(0xd, side). Then it clears
 * field_157, copies field_25c to field_4b, calls func_80142c04 and
 * returns 1.
 *
 * Contract:
 *   Argument: a0 = object. Result: v0 = 0 or 1 (int).
 *   Reads: object fields 4 to 7, 7e, 45, a6 (side), cd, 25c, 40 (partner
 *     pointer, used by the callees); game_state.field_30, .mode (byte at
 *     0x80190123), .config and from it fields 4d, 4e, 04.
 *   Writes: object fields 69, 6a, 19f, 1a2, bd, 157, 4b; then what the
 *     callees write.
 *   func_8012f56c and func_80142c04 run as the original code (they read
 *     object->field_40, a block the setup provides; the result of the first
 *     is zero when the 16-bit field_04 of that block has 1 in its low byte,
 *     which the setup arranges in about half the cases).
 *   func_80155d4c is replaced by a recorder with two arguments, no pointee,
 *     and result 0 (its result is not used); its own code reaches library
 *     and game objects outside this test. Watched at every call: the whole
 *     object and ref_other.
 *   Aliasing: object, partner block and configuration block are distinct.
 *   Exclusions: none. All instruction slots are reachable.
 * The tree declares func_80141cec with a byte result (u8); the result is 0 or 1.
 * The tree declares func_80155d4c with int arguments; the side is cast to s8 at the call, as the original passes it sign-extended.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142c04(Object *object);

u8 func_80141cec(Object *object) {
    int checked = 0;

    if (!(object->field_04 == 1 && object->field_05 == 1 && object->field_06 == 2 && object->field_07 == 0)) {
        checked = 1;
        if (object->field_7e != 0 || object->field_45 != 0 ||
            object->field_04 != 1 || object->field_05 != 0 ||
            object->field_06 == 7 || object->field_06 == 8 || object->field_06 == 9) {
            return 0;
        }
    }
    object->field_69 = 0;
    object->field_6a = 0;
    if (game_state.field_30 != 0) {
        if (game_state.mode != object->side + 1) {
            object->field_19f = 0;
            object->field_1a2 = 0;
        }
    }
    if ((game_state.config->field_4d | game_state.config->field_4e | game_state.config->field_04) != 0) {
        return 0;
    }
    if ((func_8012f56c(object) & 0xff) != 0) {
        return 0;
    }
    if (!checked) {
        object->field_bd = 2;
        if (object->field_cd == 0) {
            func_80155d4c(0xd, (s8)object->side);
        }
    }
    object->field_157 = 0;
    object->field_4b = object->field_25c;
    func_80142c04(object);
    return 1;
}
