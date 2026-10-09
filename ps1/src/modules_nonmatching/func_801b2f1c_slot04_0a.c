/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size, instruction
 * scheduling and register choice. The exact owner of the bytes in the PS1
 * build stays the raw bytes of the module image; the build does not use
 * this file. The differential test next to it (difftest.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): a state of a character
 * object that starts a jump away from or towards the opponent. When
 * func_801b1e04_slot04_0a returns non-zero (low 16 bits) it calls
 * func_801b30a0_slot04_0a and returns. Otherwise, when the object's
 * field_50 is not above 0x50000 (signed) and one of the bits 0x80, 0x10, 4
 * or 1 (tested in that order) of its halfword at 0x134 is set, it selects
 * a number (0 for 0x80 and 1, 1 for 0x10, 2 for 4), adds one to field_07,
 * starts the sequence 0x2b plus that number with func_801307e0, sets
 * field_0b to 1 when the opponent's position x (object->other) is not less
 * than its own, sets the velocity fields (field_4c, 0x70000 or -0x70000
 * towards that side; field_50 0x38000; field_54, -0x4000 or 0x4000; field_58
 * -0x4000), and calls func_801204f4, func_80141f28 and func_80138ae8. Then,
 * also when no bit was set or field_50 was too large, it calls
 * func_80130efc.
 *
 * Contract (what the code reads and writes):
 *   Argument: a0 = pointer to an object (0x394 bytes). No return value.
 *   Reads: the object's field_50, the halfword at 0x134, field_07, field_12
 *     (position x), the pointer at 0x40 (the opponent object) and its
 *     field_12, and the byte at 0xa6 (side); game_state only by address.
 *   Writes: the object's field_07, field_0b, field_4c, field_50, field_54
 *     and field_58.
 *   Callees replaced by recorders (all of them; each steps animation or
 *     object state that would need a contract of its own):
 *     func_801b1e04_slot04_0a (1 argument; returns 0 or a random non-zero
 *     value), func_801b30a0_slot04_0a (1), func_801307e0 (2, the second
 *     under a 16-bit mask), func_801204f4 (3, the second and third as the
 *     function passes them), func_80141f28 (2), func_80138ae8 (2, the
 *     address of game_state and the object), func_80130efc (1); all return 0
 *     except the first. The log watches the whole object and the whole
 *     opponent object, so that a field written after a call instead of
 *     before it is a difference.
 *   Aliasing: the object and the opponent are distinct blocks.
 *   Excluded inputs: none.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations, not original ones. */
u16 func_801b1e04_slot04_0a(Object *obj);
void func_801b30a0_slot04_0a(Object *obj);

void func_801b2f1c_slot04_0a(Object *obj) {
    int variant;
    u16 buttons;
    u8 facing;
    int vx;
    int vy;

    if (func_801b1e04_slot04_0a(obj)) {
        func_801b30a0_slot04_0a(obj);
        return;
    }
    if (obj->field_50 <= 0x50000) {
        buttons = obj->field_134;
        if (buttons & 0x80) {
            variant = 0;
        } else if (buttons & 0x10) {
            variant = 1;
        } else if (buttons & 4) {
            variant = 2;
        } else if (buttons & 1) {
            variant = 0;
        } else {
            goto end;
        }
        obj->field_07++;
        func_801307e0(obj, variant + 0x2b);
        facing = !(obj->other->pos_x < obj->pos_x);
        obj->field_0b = facing;
        if (facing) {
            vx = -0x70000;
            vy = 0x4000;
        } else {
            vx = 0x70000;
            vy = -0x4000;
        }
        obj->field_50 = 0x38000;
        obj->field_54 = vy;
        obj->field_4c = vx;
        obj->field_58 = -0x4000;
        func_801204f4(obj, obj->side, 9);
        func_80141f28(obj, 2);
        func_80138ae8(&game_state, obj);
    }
end:
    func_80130efc(obj);
}
