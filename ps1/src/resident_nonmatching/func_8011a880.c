/*
 * Nonmatching. This function is NOT byte-identical to the original: it is
 * written for readability and its build differs from the original's bytes.
 * The build does not use this file. The differential test next to it
 * (difftest.py, with func_8011a880.py) compares the behavior of this C with
 * the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): for the two fighters
 * (player_left, player_right) it decides which one is drawn in front and
 * stores that as field_09 (1 for the one in front, 0 for the other), then
 * runs the per-fighter update calls for the left fighter and for the right
 * one. If either fighter has a non-zero field_73, the one with the larger
 * field_17a is in front (the right one when they are equal and the right
 * one's field_73 is 1, the left one when they are equal otherwise). If both
 * have field_73 equal to 0, the one whose current frame record has the
 * larger field_04 is in front, and when those are equal neither field_09 is
 * changed. Then, for each fighter in turn: the four part blocks at
 * offsets 0x28, 0x2c, 0x30 and 0x34, when present and with field_01 not 0,
 * get field_09 = fighter's field_09 + 4 and are passed to func_8011bc84; the
 * calls to func_8011cbb8(fighter) (after the 0x2c part) and to
 * func_80151324(fighter, frames) (at the end) happen when the fighter's
 * field_00 and field_01 are both not 0.
 *
 * Contract:
 *   No argument, no result. Reads and writes player_left and player_right
 *     (field_00, field_01, field_09, field_73, the frame pointer at 0x88,
 *     field_17a, the part pointers), the frame records they point at
 *     (field_04), frames_left and frames_right (scratchpad words), and
 *     field_01 and field_09 of each part block.
 *   Writes: field_09 of both fighters (before the calls) and field_09 of
 *     each part block it passes to func_8011bc84.
 *   Callees replaced by recorders in the test, returning 0: func_8011bc84
 *     (1 argument), func_8011cbb8 (1 argument), func_80151324 (2 arguments).
 *     What they do is outside the test; the order and arguments of the
 *     calls are compared. At every call the test also copies both fighters
 *     whole (watched), and func_8011bc84 records the 0xac bytes of the
 *     block it is given, so a store made after a call instead of before
 *     it is a difference.
 *   Aliasing: the fighters are the two globals, each part block and each
 *     frame record is its own block.
 *   Excluded inputs: a null frame pointer when both field_73 are 0 (the
 *     original reads through it); the setup always makes the frame
 *     pointers valid.
 *   Not reached by any input: none known (see the coverage line).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* View of a fighter record (an Object) with the fields this function uses;
   offsets inferred from the code, not an original declaration. */
typedef struct {
    u8 field_00;
    u8 field_01;
    u8 pad_02[7];
    u8 field_09;
    u8 pad_0a[0x28 - 0x0a];
    Slab172 *part_28;
    Slab172 *part_2c;
    Slab172 *part_30;
    Slab172 *part_34;
    u8 pad_38[0x73 - 0x38];
    u8 field_73;
    u8 pad_74[0x88 - 0x74];
    FrameRecord *frame;
    u8 pad_8c[0x17a - 0x8c];
    u8 field_17a;
} FighterView;

void func_8011cbb8(Slab172 *p);
void func_80151324(Object *object, FrameRecord *frames);

/* A part block that is present and active gets the fighter's field_09 plus 4
   and is handed to func_8011bc84. */
static void update_part(Slab172 *p, FighterView *f) {
    if (p != 0 && p->field_01 != 0) {
        p->field_09 = f->field_09 + 4;
        func_8011bc84(p);
    }
}

static void update_fighter(FighterView *f, FrameRecord *frames) {
    update_part(f->part_28, f);
    update_part(f->part_2c, f);
    if (f->field_00 != 0 && f->field_01 != 0) {
        func_8011cbb8((Slab172 *)f);
    }
    update_part(f->part_30, f);
    update_part(f->part_34, f);
    if (f->field_00 != 0 && f->field_01 != 0) {
        func_80151324((Object *)f, frames);
    }
}

void func_8011a880(void) {
    FighterView *l = (FighterView *)&player_left;
    FighterView *r = (FighterView *)&player_right;
    u8 a, b;

    if (l->field_73 != 0 || r->field_73 != 0) {
        l->field_09 = 0;
        r->field_09 = 0;
        if (l->field_17a < r->field_17a || (l->field_17a == r->field_17a && r->field_73 == 1)) {
            r->field_09 = 1;
        } else {
            l->field_09 = 1;
        }
    } else {
        a = l->frame->field_04;
        b = r->frame->field_04;
        if (a > b) {
            l->field_09 = 0;
            r->field_09 = 1;
        } else if (b > a) {
            l->field_09 = 1;
            r->field_09 = 0;
        }
    }
    update_fighter(l, frames_left);
    update_fighter(r, frames_right);
}
