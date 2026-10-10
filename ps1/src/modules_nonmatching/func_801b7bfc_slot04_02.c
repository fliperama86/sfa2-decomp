/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 8 bytes shorter and orders some loads and stores
 * differently (the original reads field_03 once, early, and stores field_09
 * twice; read from the original's listing, not tested). The exact owner of
 * the bytes in the PS1 build stays the raw bytes
 * of the module image; the build does not use this file. The differential test next to it
 * (difftest.py, with func_801b7bfc_slot04_02.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a state of a helper object
 * that follows another one (field_3c). It advances its state byte
 * (field_04). While game_state.field_64 is not 0 it copies the other
 * object's field_1c, field_90, field_98, field_9c, field_7a and field_7c,
 * places itself at fixed coordinates (x from box_margin, y from
 * data_801aa5ea), steps field_03 up by one
 * unless the other object's field_5c is 0x90, loads a palette copy for that
 * value (func_801b7e34), calls func_80130768 with 8 and one of two tables
 * (the scratchpad word data_1f8000b4 when the other object's side is 0,
 * else data_1f800164) and registers itself (func_8011ffdc). When field_64
 * is 0 it only calls func_801b7d58_slot04_02, which sets field_04 to 2
 * (read from the original's listing, not tested).
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: field_04, field_3c (a second object, distinct from the first),
 *     field_03; the second object's field_1c, field_90, field_98, field_9c,
 *     field_7a, field_7c, field_5c and side; game_state.field_64; box_margin;
 *     data_801aa5ea; the scratchpad words data_1f8000b4 and data_1f800164
 *     (pointers to tables of sequence pointers; each is passed to
 *     func_80130768 together with 8).
 *   Writes: field_04 (+1, or 2 in the idle arm), and in the active arm
 *     field_0f, field_0e, field_0b = 0, field_09 = 6, field_1c, field_90,
 *     field_98, field_9c, field_7a, field_7c, pos_x, field_0d = 0x1f,
 *     field_44 = 1, field_46 = 0x15, field_54 = 6, field_a0 = 0, pos_y,
 *     field_03, the palette array data_801a2bc4 (written by
 *     func_801b7e34_slot04_02), and what the other callees write.
 *   Callees: func_801b7d58_slot04_02, func_801b7e34_slot04_02, func_80130768
 *     and func_8011ffdc run as the original code (with the setup giving them
 *     sequence tables, frame records and the object stacks). The one callee
 *     replaced is func_80137220 (2 arguments), called by func_801b7e34: it
 *     reaches the job pool and the video code of the resident image (read
 *     from the original's listing, not tested); the recorder returns a
 *     random word.
 *   Aliasing: the two objects, tables, steps, frame records and stacks are
 *     distinct blocks.
 *   Watched by the recorders: the whole object (0x394 bytes) and the array data_801a2bc4 (8 words) at the call of the recorder,
 *     so the order of this function's stores against the calls is tested.
 *     No recorded callee gets a pointer to memory filled for the call.
 *   Inputs excluded: none. Slots not reached: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;

/* Declared here because the published headers lack them (inferred). */
void func_801b7d58_slot04_02(Object *obj);
void func_801b7e34_slot04_02(Object *obj, u8 arg);

void func_801b7bfc_slot04_02(Object *obj) {
    Object *other;
    u8 palette;

    obj->field_04++;
    other = obj->field_3c;
    if (game_state.field_64 != 0) {
        obj->field_0f = 0;
        obj->field_0e = 0;
        obj->field_0b = 0;
        obj->field_09 = 6;
        obj->field_1c = other->field_1c;
        obj->field_90 = other->field_90;
        obj->field_98 = other->field_98;
        obj->field_9c = other->field_9c;
        obj->field_7a = other->field_7a;
        obj->field_7c = other->field_7c;
        obj->pos_x = box_margin[0] + 0xb8;
        obj->field_0d = 0x1f;
        obj->field_44 = 1;
        obj->field_46 = 0x15;
        obj->field_54 = 6;
        obj->field_a0 = 0;
        obj->pos_y = 0x78 - data_801aa5ea[0];
        palette = obj->field_03;
        if (other->field_5c != 0x90) {
            palette++;
        }
        obj->field_03 = palette;
        func_801b7e34_slot04_02(obj, palette);
        if (other->side == 0) {
            func_80130768(obj, 8, data_1f8000b4);
        } else {
            func_80130768(obj, 8, data_1f800164);
        }
        func_8011ffdc(obj);
    } else {
        func_801b7d58_slot04_02(obj);
    }
}
