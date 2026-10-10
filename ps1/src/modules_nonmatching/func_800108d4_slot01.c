/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * (a reload of a global sits at another place). The exact owner of the
 * bytes in the PS1 build stays the raw bytes of the module image; the
 * build does not use this file. The differential test next to it
 * (difftest.py, with func_800108d4_slot01.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): draws the status panel
 * of the object that game_state.field_78 points at, unless that object's
 * field_cd is not 0. It keeps that object in data_8002cea8_slot01 and
 * picks a left position data_8002cea4_slot01 (0x10 or 0xe0, by the
 * object's field_65). A table of eight text records
 * (data_800150cc_slot01, 0x10 bytes each: a halfword x at offset 4 and a
 * text buffer pointer at 0xc) is used for seven of them: x is set for
 * records 0 to 4 and 6, and for record 5 when field_cc is not 0 (for
 * record 7 otherwise). Two numbers are formatted into the text buffers,
 * three when field_cc is not 0: by func_80010d0c_slot01 (the object's
 * field_e8, into record 1) and func_80010bf0_slot01 (field_d7 or field_c0
 * depending on game_state.field_85, into record 3, and field_cc into
 * record 5 when it is not 0). Each of the seven records is queued for
 * drawing by func_801519b4. When field_cc is 0 record 7 is used with x
 * 0x78 or 0x148 (by field_65) instead of record 5.
 * Last, data_8002cea4_slot01 is set to 0x88 or 0x158 (by field_65) and
 * field_ce + 1 strips are filled in, taken from the row data_801a27d0 of
 * the array data_80190014 (field_65 not 0) or of the array strips
 * (field_65 0): x = that value minus 16 times the countdown (which runs
 * from field_ce to 0), y = 0x6e; each is linked into the ordering table
 * head of the buffer data_801a27d0.
 *
 * Contract:
 *   No argument, no return value.
 *   Reads: game_state.field_78 (the object), field_85; the object's
 *     field_65, field_c0, field_cc, field_cd, field_ce, field_d7 and
 *     field_e8; the buffer selector data_801a27d0 (0 or 1); the text
 *     buffer pointer (offset 0xc) of records 1 and 3, and of record 5
 *     when field_cc is not 0; the rows of the arrays data_80190014 and
 *     strips (the pointer to the first strip); the head words of the
 *     ordering table at data_801fc050 (232 bytes per buffer).
 *   Writes: data_8002cea8_slot01, data_8002cea4_slot01,
 *     data_8002cea0_slot01 (ends at -1), data_8002ce9c_slot01 (ends
 *     after the last strip); the x halfword (offset 4) of records 0 to 4
 *     and 6, of record 5 when field_cc is not 0, and of record 7 when
 *     field_cc is 0 (their x only); per strip the link
 *     word (low 24 bits), and halfwords at 0x14 and 0x16; the head word
 *     of the ordering table (low 24 bits); and what func_801519b4 writes
 *     (the queue table_8018d144, its counter data_8018d204).
 *   Callees: func_80010d0c_slot01 and func_80010bf0_slot01 (both reach the
 *     library) are recorders, two arguments each (the number and the
 *     buffer pointer), result 0. The log copies at every call the whole
 *     record array and the four words data_8002ce9c_slot01 to
 *     data_8002cea8_slot01 (the function writes data_8002cea8_slot01 and
 *     data_8002cea4_slot01 before the calls, data_8002cea0_slot01 and
 *     data_8002ce9c_slot01 after the last one; the setup fills all four
 *     with random bytes).
 *     func_801519b4 (queue a pointer) runs as the original code, with its
 *     queue in memory.
 *   Aliasing: the object, the records' text buffers, the strips and the
 *     ordering table are distinct from each other; no callee changes
 *     data_8002cea8_slot01.
 *   Excluded inputs: field_ce above 3 (a row of strips holds four).
 *   Not reached by any input: none (every instruction slot of the original
 *     is executed). The loop's entry test for a negative field_ce has no
 *     input, since field_ce is a byte, but the test's instruction runs.
 *   The setup writes the records of the module image (random bytes), the
 *     same for both runs.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A text record of the table: 0x10 bytes. Inferred from the code; not an
   original declaration. */
typedef struct {
    u32 pad0;
    u16 x;
    u16 pad6;
    u32 pad8;
    char *text;
} Slot01TextRec;

extern Slot01TextRec data_800150cc_slot01[];
extern Object *data_8002cea8_slot01;
extern int data_8002cea4_slot01;
extern int data_8002cea0_slot01;
extern Strip1c *data_8002ce9c_slot01;
extern u32 data_801fc050[];
void func_80010d0c_slot01(int value, char *buf);
void func_80010bf0_slot01(int value, char *buf);

void func_800108d4_slot01(void) {
    Object *o = game_state.field_78;
    Slot01TextRec *last;
    Strip1c *strip;
    u32 *head;
    int n;

    data_8002cea8_slot01 = o;
    if (o->field_cd != 0) return;

    data_8002cea4_slot01 = o->field_65 != 0 ? 0x10 : 0xe0;
    data_800150cc_slot01[0].x = data_8002cea4_slot01;
    data_800150cc_slot01[1].x = data_8002cea4_slot01 + 0x38;
    func_80010d0c_slot01(o->field_e8, data_800150cc_slot01[1].text);
    func_801519b4((Object *)&data_800150cc_slot01[0]);
    func_801519b4((Object *)&data_800150cc_slot01[1]);

    data_800150cc_slot01[2].x = data_8002cea4_slot01;
    data_800150cc_slot01[3].x = data_8002cea4_slot01 + 0x48;
    func_80010bf0_slot01(game_state.field_85 != 0 ? o->field_d7 : o->field_c0, data_800150cc_slot01[3].text);
    func_801519b4((Object *)&data_800150cc_slot01[2]);
    func_801519b4((Object *)&data_800150cc_slot01[3]);

    data_800150cc_slot01[4].x = data_8002cea4_slot01;
    func_801519b4((Object *)&data_800150cc_slot01[4]);

    if (o->field_cc != 0) {
        data_800150cc_slot01[5].x = data_8002cea4_slot01 + 0x48;
        func_80010bf0_slot01(o->field_cc, data_800150cc_slot01[5].text);
        last = &data_800150cc_slot01[5];
    } else {
        data_800150cc_slot01[7].x = o->field_65 != 0 ? 0x78 : 0x148;
        last = &data_800150cc_slot01[7];
    }
    func_801519b4((Object *)last);

    data_800150cc_slot01[6].x = data_8002cea4_slot01;
    func_801519b4((Object *)&data_800150cc_slot01[6]);

    if (o->field_65 != 0) {
        data_8002cea4_slot01 = 0x88;
        strip = data_80190014[data_801a27d0];
    } else {
        data_8002cea4_slot01 = 0x158;
        strip = strips[data_801a27d0];
    }
    head = data_801fc050 + data_801a27d0 * 58;
    for (n = o->field_ce; n >= 0; n--) {
        strip->field_14 = data_8002cea4_slot01 - n * 16;
        strip->field_16 = 0x6e;
        ((PrimTag *)strip)->addr = ((PrimTag *)head)->addr;
        ((PrimTag *)head)->addr = (u32)strip;
        strip++;
    }
    data_8002cea0_slot01 = n;
    data_8002ce9c_slot01 = strip;
}
