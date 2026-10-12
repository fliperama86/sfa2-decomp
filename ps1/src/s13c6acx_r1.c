/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* What it does (inferred, not an original name): for each object of the
   object table (data_8018f5e0, read downward, count_8018f59c entries) of
   kind 9 and type 1 it picks a player object (player_left, or player_right
   when the object's field_65 is 0) and pushes the player out of the
   object's box. It compares the player's box (taken from the player's
   unknown_148 table at the frame's box_index) with the object's "c" box
   (boxes_c of the object's box_tables at its frame's box_index): when the
   vertical gap is not smaller than the sum of the box extents (field_05) it
   does nothing; otherwise it measures the horizontal overlap and, when
   there is one, moves the player sideways by that overlap (field_164 of the
   player zero), or the player's other object and the table object both by
   the overlap in the other direction (field_164 not zero). */
void func_8013c6ac(void)
{
    s16 last;
    s16 i;
    int d;
    int fi;
    Box6 *obj_box;
    Box6 *player_box;
    Box6 *pbase;
    u16 obj_x;

    /* Reading count_8018f59c once into last and decrementing it afterwards: the plain "last = count_8018f59c - 1" after the test builds 712 bytes and differs in 9 instruction slots. */
    last = count_8018f59c;
    if (last == 0) {
        return;
    }
    last--;
    ((ObjectStackRef *)&ref_other)->pp = (Object **)data_8018f5e0;
    for (i = 0; i <= last; i++) {
        ref_third.p = *((ObjectStackRef *)&ref_other)->pp--;
        if (ref_third.p->field_02 != 9) {
            continue;
        }
        if (ref_third.p->field_00 != 1) {
            continue;
        }
        ref_second.p = &player_left;
        if (ref_third.p->field_65 == 0) {
            ref_second.p = &player_left + 1;
        }
        if (ref_second.p->field_45 != 0) {
            continue;
        }
        if (ref_third.p->frame->box_index == 0) {
            continue;
        }
        data_80190414.p = (Object *)ref_third.p->box_tables;
        obj_box = ((BoxTables *)data_80190414.p)->boxes_c;
        obj_box += ref_third.p->frame->box_index;
        if (ref_second.p->frame->box_index == 0) {
            continue;
        }
        /* pbase holds the loaded pointer that is stored to data_80190458 and then stepped into player_box: with the loaded pointer stepped in player_box itself the unit differs in 33 instruction slots. */
        pbase = (Box6 *)ref_second.p->unknown_148;
        data_80190458.p = (Object *)pbase;
        player_box = pbase + ref_second.p->frame->box_index;
        /* fi holds the box's origin_y here and its other values below (one variable for several values): with a separate local for origin_y the unit builds 712 bytes and differs in 54 instruction slots. */
        fi = obj_box->origin_y;
        d = (s16)player_box->origin_y;
        d -= ref_second.p->pos_y;
        d -= (s16)(fi - ref_third.p->pos_y);
        if (d < 0) {
            d = -d;
        }
        d -= (player_box->field_05 + obj_box->field_05);
        if (d >= 0) {
            continue;
        }
        fi = (u16)obj_box->origin;
        if (ref_third.p->field_0b) {
            fi = -fi;
        }
        obj_x = fi + ref_third.p->pos_x;
        d = player_box->origin;
        if (ref_second.p->field_0b) {
            d = -d;
        }
        fi = 0;
        d += ref_second.p->pos_x;
        d -= (s16)obj_x;
        if (d < 0) {
            d = -d;
            fi = 1;
        }
        d = d - (player_box->extent + obj_box->extent);
        if (d >= 0) {
            continue;
        }
        d = -d;
        if (fi) {
            d = -d;
        }
        if (ref_second.p->field_164 != 0) {
            ref_first.p = ref_second.p->other;
            ref_first.p->pos_x = ref_first.p->pos_x - d;
            ref_third.p->pos_x = ref_third.p->pos_x - d;
        } else {
            /* The second read of pos_x is wanted: with a local set from the first read and used here the unit builds 704 bytes and differs in 16 instruction slots. */
            ref_second.p->pos_x = ref_second.p->pos_x + d;
        }
    }
}
