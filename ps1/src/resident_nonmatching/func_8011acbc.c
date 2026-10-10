/*
 * Nonmatching. This function is NOT byte-identical to the original: it is
 * written for readability and its build differs from the original's bytes.
 * The build does not use this file. The differential test next to it
 * (difftest.py, with func_8011acbc.py) compares the behavior of this C with
 * the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): updates the 40 records of
 * the table data_801a89f4 (172 bytes each), one by one, using the byte
 * data_8019045c as the loop counter (it is 40 when the function ends). A
 * record is skipped unless its field_00 and field_01 are not 0. Then:
 *   - field_02 is 3, 0xb, 0x14, 0x27 or 0x6d and field_08 is not 0x20:
 *     type 3, 0xb or 0x14 calls func_8011cd68, 0x6d calls func_80134624,
 *     0x27 with field_01 equal to 1 calls func_801452ec; 0x27 with another
 *     field_01 calls func_8011bc84. Each of these records is then done.
 *   - otherwise, field_81 equal to 4: when data_80190568 is not 0, the
 *     stage handler selected by game_state.field_40 (0 to 18) is called
 *     with the record (a value outside 0 to 18 calls none);
 *   - otherwise, field_02 equal to 0x16 or 0x18 with field_08 equal to
 *     0x20: the parts at offsets 0x28 and 0x2c, the record itself, and the
 *     parts at 0x30 and 0x34 are passed to func_8011bc84 in that order;
 *     a part is passed only when its pointer is not null and its field_01
 *     is not 0, and gets the record's field_09 first;
 *   - otherwise func_8011bc84 is called with the record.
 *
 * Contract:
 *   No argument, no result. Reads data_80190568, game_state.field_40, the
 *     counter data_8019045c (set to 0 first) and the 40 records: field_00,
 *     field_01, field_02, field_08, field_09, field_81 and the part
 *     pointers at 0x28, 0x2c, 0x30 and 0x34 of each, and field_01 of each
 *     non-null part. Writes the counter and field_09 of the parts it
 *     passes on.
 *   Callees replaced by recorders in the test, each returning 0: the 19
 *     stage handlers (func_801e9080, func_801e9f90, func_801e99c8,
 *     func_801e9d04, func_801e99b4, func_801e8dc8, func_801e9798,
 *     func_801e9970, func_801e9b54, func_801e9840, func_801e8df0,
 *     func_801e96fc, func_801ea640, func_801e9f20, func_801e9d14,
 *     func_801e9bb0, func_801e9ef4, func_801e8fec, func_801e9114; 1
 *     argument each; they live in the stage module images), func_8011cd68,
 *     func_80134624, func_801452ec and func_8011bc84 (1 argument each).
 *     The original reads the counter and game_state.field_40 again after
 *     every call; this C reads the counter once per record and
 *     game_state.field_40 again for each handler test. The two agree when a
 *     callee does not change the counter, which is assumed (the recorders
 *     of the test never do). With the counter watched at every call, the
 *     test shows it equal in both runs at each call; the listing shows only
 *     the loop's own increment writes it, so the single read is kept.
 *     Every callee records the 0xac bytes behind its argument.
 *   Aliasing: the records, the parts and the table are distinct blocks; a
 *     part is never a record of the table.
 *   Excluded inputs: none. Not reached by any input: none known (see the
 *     coverage line).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80134624(Object *object);
void func_801452ec(Object *object);


/* A part block of a record is passed on only when it exists and is active;
   it then takes the record's field_09. */
static void update_part(Slab172 *rec, u32 part) {
    Slab172 *q = (Slab172 *)part;

    if (q != 0 && q->field_01 != 0) {
        q->field_09 = rec->field_09;
        func_8011bc84(q);
    }
}

static void update_record(Slab172 *rec) {
    u8 type;

    if (rec->field_00 == 0 || rec->field_01 == 0) {
        return;
    }
    type = rec->field_02;
    if ((type == 3 || type == 0xb || type == 0x14 || type == 0x27 || type == 0x6d)
        && rec->field_08 != 0x20) {
        if (type == 3 || type == 0xb || type == 0x14) {
            func_8011cd68((Block172 *)rec);
        } else if (type == 0x6d) {
            func_80134624((Object *)rec);
        } else if (type == 0x27 && rec->field_01 == 1) {
            func_801452ec((Object *)rec);
        } else {
            func_8011bc84(rec);
        }
        return;
    }
    if (rec->field_81 == 4) {
        if (data_80190568 != 0) {
            if (game_state.field_40 == 0) func_801e9080(rec);
            if (game_state.field_40 == 1) func_801e9f90(rec);
            if (game_state.field_40 == 2) func_801e99c8(rec);
            if (game_state.field_40 == 3) func_801e9d04(rec);
            if (game_state.field_40 == 4) func_801e99b4(rec);
            if (game_state.field_40 == 5) func_801e8dc8(rec);
            if (game_state.field_40 == 6) func_801e9798(rec);
            if (game_state.field_40 == 7) func_801e9970(rec);
            if (game_state.field_40 == 8) func_801e9b54(rec);
            if (game_state.field_40 == 9) func_801e9840(rec);
            if (game_state.field_40 == 10) func_801e8df0(rec);
            if (game_state.field_40 == 11) func_801e96fc(rec);
            if (game_state.field_40 == 12) func_801ea640(rec);
            if (game_state.field_40 == 13) func_801e9f20(rec);
            if (game_state.field_40 == 14) func_801e9d14(rec);
            if (game_state.field_40 == 15) func_801e9bb0(rec);
            if (game_state.field_40 == 16) func_801e9ef4(rec);
            if (game_state.field_40 == 17) func_801e8fec(rec);
            if (game_state.field_40 == 18) func_801e9114(rec);
        }
    } else if ((type == 0x16 || type == 0x18) && rec->field_08 == 0x20) {
        update_part(rec, rec->field_28);
        update_part(rec, rec->field_2c);
        func_8011bc84(rec);
        update_part(rec, rec->field_30);
        update_part(rec, rec->field_34);
    } else {
        func_8011bc84(rec);
    }
}

void func_8011acbc(void) {
    u8 *counter = (u8 *)data_8019045c;
    Slab172 *records = (Slab172 *)data_801a89f4;

    for (*counter = 0; *counter < 40; (*counter)++) {
        update_record(&records[*counter]);
    }
}
