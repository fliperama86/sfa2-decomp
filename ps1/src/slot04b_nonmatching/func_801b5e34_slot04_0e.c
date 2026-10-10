/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 8 bytes longer and differs in frame size and in where the
 * last argument is computed. The exact owner of the bytes in the PS1 build
 * stays the raw bytes of the module image; the build does not use this
 * file. The differential test next to it (difftest.py, with the contract
 * func_801b5e34_slot04_0e.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): the start of a character
 * action. It advances the object's state byte, takes 2 points off a
 * counter (func_80141f28), plays two sounds (func_801204f4,
 * func_80120554), clears two velocity words, and sets a launch velocity
 * from tables indexed by the object's direction byte field_48: a byte
 * table (index field_48 / 2) gives field_47, two word tables (index
 * field_48) give the velocity words field_4c and field_54, both negated
 * when field_0b is not 0. It then selects the sequence 0x27 + field_48 / 2.
 *
 * Contract (what the code reads and writes; the roles named for the
 * fields are inferred):
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's field_02, field_07, field_0b, field_48, side
 *     (field_a6); the tables data_801c6190_slot04_0e (bytes),
 *     data_801c6194_slot04_0e and data_801c6198_slot04_0e (words), left as
 *     the module image holds them; and what the callees read.
 *   Writes: field_07 (incremented), field_46 (7), field_47, field_4c,
 *     field_50 (0), field_54, field_58 (0), and what the callees write.
 *   Callees run as original code: func_80141f28 (reads field_74, field_2a2,
 *     field_d8, field_c6 and game_state.field_47, writes field_c6),
 *     func_801307e0 (the sequence tables in the scratchpad, the sequence
 *     steps and frame records; writes the object's sequence, field_38,
 *     field_3a, field_80, frame, field_4a; may step the sequence further).
 *   Callees replaced by recorders (no result, log in RAM): func_801204f4
 *     (3 arguments) and func_80120554 (3 arguments); both end in sound
 *     routines of Sony's library. func_80141f28 also calls func_80120554,
 *     so that call is recorded too.
 *   Watched by the recorders: the whole object (0x394 bytes), copied at every
 *     recorded call. No recorded callee takes a pointer to memory the
 *     function fills.
 *   Aliasing: the object, the sequence steps, the frame records and the
 *     pointer tables are distinct blocks.
 *   Not reached by any input: none known; see the test lines for the
 *     coverage.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Direction tables of the module: bytes, then two tables of words. Inferred
   from the code; not original declarations. */
extern u8 data_801c6190_slot04_0e[];
extern s32 data_801c6194_slot04_0e[];
extern s32 data_801c6198_slot04_0e[];

void func_801b5e34_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    o->field_07++;
    func_80141f28(o, 2);
    func_801204f4(o, o->side, 6);
    func_80120554(o, o->field_02, 0x324);
    o->field_50 = 0;
    o->field_58 = 0;
    obj->field_46 = 7;
    obj->field_47 = data_801c6190_slot04_0e[o->field_48 >> 1];
    o->field_4c = data_801c6194_slot04_0e[o->field_48];
    o->field_54 = data_801c6198_slot04_0e[o->field_48];
    if (o->field_0b != 0) {
        o->field_4c = -o->field_4c;
        o->field_54 = -o->field_54;
    }
    func_801307e0(o, (o->field_48 >> 1) + 0x27);
}
