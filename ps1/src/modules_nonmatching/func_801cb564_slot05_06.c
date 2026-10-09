/*
 * Nonmatching. This function is NOT byte-identical to the original: it is
 * written from the original's listing and has not been made to reproduce
 * its bytes. The PS1 build keeps the raw bytes of the module image and does
 * not use this file. The differential test next to it (difftest.py, with
 * func_801cb564_slot05_06.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not original names): the per-frame update of a
 * character state with a countdown and a push on the opponent. The
 * object's byte at 0x3a selects the mode.
 *   Mode 2: when the object's x is between 0x201 and 0x300 it only hands
 *     over to func_80130efc; otherwise it sets field_0b to 1 when x is
 *     below 0x280 and to 0 when not, and hands over.
 *     (Where it sets field_0b, x lies outside 0x201 to 0x300, so any limit
 *     from 0x201 to 0x301 in place of 0x280 gives the same result.)
 *   Mode 0: it only hands over to func_80130efc.
 *   Other modes, field_4c not negative: it moves the object
 *     (func_801cc814_slot05_06). When field_cd is not 0 it calls
 *     func_80141248, then func_801410c8, and a non-zero low byte of the
 *     result takes the "enter" exit below. Otherwise, when bit 0 of
 *     game_state.field_1d is 0 and (field_1c5 is below (field_12a >> 1) + 3
 *     or has its bit 0x80 set) and func_801cded4_slot05_06 returns non-zero
 *     (it makes an object and sets ref_other.p to it), it pushes that
 *     object: a random 0 to 15 plus 0x5c goes through func_801cc870_slot05_06
 *     (which negates it when field_0b is 0) and is added to the object's x,
 *     and a random 0 to 15 is subtracted from its y. Then, when bits 0 and
 *     1 of game_state.field_1d are both 0, it decides to push again: with
 *     field_1c5 equal to 1 when func_801468f4 returns a non-zero low byte,
 *     otherwise (any other field_1c5) when func_80148e84 (random 0 in 16)
 *     or func_80148ea8 (the other 15 in 16) returns a non-zero low byte;
 *     the second push uses 0x60 instead of the random amount. At the end it
 *     hands over to func_80130efc.
 *   Other modes, field_4c negative: it subtracts one from field_1c5; at 0
 *     it takes the "enter" exit. Otherwise it calls func_80140cd8,
 *     func_801204f4 and func_80120554 (for the opponent, with 0x34f when the
 *     opponent's field_5c is negative and 0x34e when not), sets field_4c to
 *     0xa0000, field_54 to -0xe000 and game_state.field_63 to 8, and starts
 *     the sequence that the table data_801dd478_slot05_06 holds for
 *     field_1c5.
 *   "Enter" exit: adds one to field_07 and starts the sequence 0x2e with
 *     func_801307e0.
 *
 * Contract (what the code reads and writes):
 *   Argument: a0 = pointer to an object (0x394 bytes). No return value.
 *   Reads: the object's field_3a (byte), x, field_4c, field_cd, field_12a,
 *     field_1c5, field_0b, side (0xa6), pointer field_40 (the opponent) and
 *     the opponent's field_5c and side; game_state.field_1d; ref_other.p
 *     (set by the setup, because the recorder of func_801cded4_slot05_06
 *     does not make the object); the table data_801dd478_slot05_06 (256
 *     halfwords); the random generator state.
 *   Writes: the object's field_0b, field_07, field_1c5, field_4c, field_54;
 *     ref_other.p's x and y; game_state.field_63; the random generator
 *     state; and what the real callees write (func_801cc814_slot05_06
 *     writes the object's words at 0x10 and 0x4c).
 *   Callees that run as the original: func_801cc814_slot05_06 (the move),
 *     func_801cc870_slot05_06 (negates a halfword by field_0b) and
 *     func_80151184 (random generator).
 *   Callees replaced by recorders: func_80141248 (1 argument),
 *     func_801410c8 (1, random result), func_801cded4_slot05_06 (1, random
 *     result), func_801468f4 (1, random result), func_80148e84 (1, random
 *     result), func_80148ea8 (1, random result), func_80140cd8 (3),
 *     func_801204f4 (3), func_80120554 (3), func_801307e0 (2, the second
 *     under a 16-bit mask), func_80130efc (1). They reach resident code of
 *     the objects, collision and animation. The log watches the whole
 *     object, ref_other.p's words at offsets 0x10 and 0x14 (x and y, which
 *     this function writes after calls) and the word at game_state offset
 *     0x60 (field_63), so that a field written after a call instead of
 *     before it is a difference.
 *   Aliasing: the object, the opponent and the object at ref_other.p are
 *     distinct blocks.
 *   Excluded inputs: none; field_4c and its move are kept within 30 bits so
 *     that the move's sum does not overflow (the move is signed).
 *   Not reached by any input: the original has 4 instruction slots (at
 *     offsets 0x8c to 0x98) that set game_state.field_63 to 0x18 and call
 *     func_80146960 when the mode byte is 2; the same byte was already
 *     found not 2 at the first test (mode 2 returns before), and the
 *     original does not re-read it, so no input reaches them. They are not
 *     written in this C.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations, not original ones. */
void func_801cc814_slot05_06(Object *obj);
void func_801cc870_slot05_06(Object *obj, u16 *amount);
int func_801cded4_slot05_06(Object *obj);
void func_80141248(Object *object);
int func_801410c8(Object *object);
int func_801468f4(Object *object);
u8 func_80140cd8(Object *object, int a, int b);
extern u16 data_801dd478_slot05_06[];

/* The push on the opponent object, done twice with another amount. */
static void push_other(Object *obj, u16 amount) {
    func_801cc870_slot05_06(obj, &amount);
    ref_other.p->pos_x += amount;
    ref_other.p->pos_y -= func_80151184() & 0xf;
}

void func_801cb564_slot05_06(Object *obj) {
    Slot04aObj *self = (Slot04aObj *)obj;
    Object *other;
    u8 pushed;

    if (self->field_3a == 2) {
        if (obj->pos_x > 0x200 && obj->pos_x < 0x301) {
            func_80130efc(obj);
            return;
        }
        obj->field_0b = 0;
        if (obj->pos_x < 0x280) {
            obj->field_0b = 1;
        }
        func_80130efc(obj);
        return;
    }
    if (self->field_3a == 0) {
        func_80130efc(obj);
        return;
    }

    if (obj->field_4c < 0) {
        self->field_1c5--;
        if (self->field_1c5 == 0) goto enter;
        func_80140cd8(obj, -0xfe, 0);
        func_801204f4(obj, obj->side, 8);
        other = obj->other;
        if (((Slot04aObj *)other)->field_5c < 0) {
            func_80120554(other, other->side, 0x34f);
        } else {
            func_80120554(other, other->side, 0x34e);
        }
        obj->field_4c = 0xa0000;
        obj->field_54 = -0xe000;
        game_state.field_63 = 8;
        func_801307e0(obj, data_801dd478_slot05_06[self->field_1c5]);
        return;
    }

    func_801cc814_slot05_06(obj);
    if (obj->field_cd != 0) {
        func_80141248(obj);
        if ((u8)func_801410c8(obj) != 0) goto enter;
    }
    if ((game_state.field_1d & 1) == 0) {
        if (self->field_1c5 < (obj->field_12a >> 1) + 3 || (self->field_1c5 & 0x80) != 0) {
            if (func_801cded4_slot05_06(obj) != 0) {
                push_other(obj, (func_80151184() & 0xf) + 0x5c);
            }
        }
    }
    if ((game_state.field_1d & 3) == 0) {
        if (self->field_1c5 == 1) {
            pushed = func_801468f4(obj);
        } else if ((func_80151184() & 0xf) != 0) {
            pushed = func_80148ea8(obj);
        } else {
            pushed = func_80148e84(obj);
        }
        if (pushed != 0) {
            push_other(obj, 0x60);
        }
    }
    func_80130efc(obj);
    return;

enter:
    obj->field_07++;
    func_801307e0(obj, 0x2e);
}
