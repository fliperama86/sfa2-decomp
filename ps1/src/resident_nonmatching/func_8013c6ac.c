/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * build keeps the original bytes of the resident executable and does not use
 * this file. The differential test next to it (difftest.py, with
 * func_8013c6ac.py) compares the behavior of this C with the original code
 * on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): for each object of the
 * object table (data_8018f5e0, read downward, count_8018f59c entries) of
 * kind 9 and type 1 it picks a player object (player_left, or player_right
 * when the object's field_65 is 0) and pushes the player out of the
 * object's box. It compares the player's box (taken from the player's
 * unknown_148 table at the frame's field_07) with the object's "c" box
 * (boxes_c of the object's box_tables at its frame's field_07): when the
 * vertical gap is not smaller than the sum of the box extents (field_05) it
 * does nothing; otherwise it measures the horizontal overlap and, when
 * there is one, moves the player sideways by that overlap (field_164 of the
 * player zero), or the player's other object and the table object both by
 * the overlap in the other direction (field_164 not zero).
 *
 * Contract (the roles named for the fields are inferred):
 *   No argument, no return value.
 *   Reads: count_8018f59c (16 bits, signed), the table data_8018f5e0 (words
 *     at 0, -4, -8, ... from its start), the table objects' field_00,
 *     field_02, field_65, frame, box_tables, pos_x, pos_y, field_0b; the
 *     players' field_45, frame (field_07), unknown_148, pos_x, pos_y,
 *     field_0b, field_164, other; the box records (6 bytes: origin at 0,
 *     field_02 at 2, extent at 4, field_05 at 5); the boxes_c word of the
 *     box_tables (offset 8).
 *   Writes: ref_other (a pointer into the table), ref_third (the table
 *     object), ref_second (the player), data_80190414 (the object's
 *     box_tables), data_80190458 (the player's unknown_148), ref_first (the
 *     player's other object; set only when the player's field_164 is not
 *     0), and pos_x: the player's when its field_164 is 0, else the
 *     player's other object's and the table object's.
 *   Callees: none.
 *   Aliasing: the table objects, players' frames, box tables and box lists
 *     are distinct blocks, except that the table may hold player_left or
 *     player_right, and the player's other object may be any table object
 *     or the table object or a player; the 16-bit writes go to pos_x only.
 *   Excluded: a count above the size of the table (the original reads
 *     the memory below the symbol; the table holds at most 12 entries).
 *   Not reached by any input: none expected; see the coverage line.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
#include "../object.h"

/* Globals as the original uses them (inferred types; not original
   declarations). */
extern ObjectRef ref_third;
extern ObjectRef data_80190414;
/* data_80190458 is an ObjectRef in the shared header; the function uses its
   word as a pointer to Box6 records. */

void func_8013c6ac(void) {
    s16 last;
    s16 i;
    int dy;
    int dx;
    int flip;
    int off_m;
    int off_o;
    Object *player;
    Object *obj;
    Box6 *obj_box;
    Box6 *player_box;
    u16 obj_x;

    if (count_8018f59c == 0) return;
    ((ObjectStackRef *)&ref_other)->pp = (Object **)data_8018f5e0;
    last = count_8018f59c - 1;
    for (i = 0; i <= last; i++) {
        ref_third.p = *((ObjectStackRef *)&ref_other)->pp--;
        if (ref_third.p->field_02 != 9) continue;
        if (ref_third.p->field_00 != 1) continue;
        ref_second.p = &player_left;
        if (ref_third.p->field_65 == 0) ref_second.p = &player_right;
        player = ref_second.p;
        if (player->field_45 != 0) continue;
        obj = ref_third.p;
        if (obj->frame->field_07 == 0) continue;
        data_80190414.p = (Object *)obj->box_tables;
        obj_box = ((BoxTables *)data_80190414.p)->boxes_c + obj->frame->field_07;
        if (player->frame->field_07 == 0) continue;
        data_80190458.p = (Object *)player->unknown_148;
        player_box = (Box6 *)data_80190458.p + player->frame->field_07;
        dy = (s16)player_box->field_02 - player->pos_y - (s16)(obj_box->field_02 - obj->pos_y);
        if (dy < 0) dy = -dy;
        if (dy - (player_box->field_05 + obj_box->field_05) >= 0) continue;
        off_m = (u16)obj_box->origin;
        if (obj->field_0b) off_m = -off_m;
        obj_x = off_m + obj->pos_x;
        off_o = player_box->origin;
        if (player->field_0b) off_o = -off_o;
        flip = 0;
        dx = off_o + player->pos_x - (s16)obj_x;
        if (dx < 0) {
            dx = -dx;
            flip = 1;
        }
        dx = dx - (player_box->extent + obj_box->extent);
        if (dx >= 0) continue;
        dx = -dx;
        if (flip) dx = -dx;
        if (player->field_164 != 0) {
            ref_first.p = player->other;
            ref_first.p->pos_x = ref_first.p->pos_x - dx;
            ref_third.p->pos_x = ref_third.p->pos_x - dx;
        } else {
            player->pos_x = player->pos_x + dx;
        }
    }
}
