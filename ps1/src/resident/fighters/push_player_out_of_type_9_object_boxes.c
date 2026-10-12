/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* What it does (inferred, not an original name): for each object of the
   object table (data_8018f5e0, read downward, count_8018f59c entries) of
   kind 9 and type 1 it picks a player object (player_left, or player_right
   when the object's team_id is 0) and pushes the player out of the
   object's box. It compares the player's box (taken from the player's
   unknown_148 table at the frame's box_index) with the object's "c" box
   (boxes_c of the object's box_tables at its frame's box_index): when the
   vertical gap is not smaller than the sum of the box extents (extent_y) it
   does nothing; otherwise it measures the horizontal overlap and, when
   there is one, moves the player sideways by that overlap (bound_push_flag of the
   player zero), or the player's other object and the table object both by
   the overlap in the other direction (bound_push_flag not zero). */
void push_player_out_of_type_9_object_boxes(void)
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
        if (ref_third.p->active_count != 1) {
            continue;
        }
        ref_second.p = &player_left;
        if (ref_third.p->team_id == 0) {
            /* Stand-in, not recovered source: the address of the right player formed from the left player's address below reproduces the original's choice of registers. Without it this function differs from the original in 21 instruction slots. */
            /* The figure's control writes &player_right; the build is then 712 bytes. The original forms this address by adding the size of an Object to the left player's. The sum is taken on the address as a 64-bit integer, because C pointer arithmetic may not step from one object into another. It relies on two things that hold on the PS1 and must hold in the PC program: flat addresses, and player_right placed directly behind player_left. It is not portable C in general. */
            ref_second.p = (Object *)((unsigned long long)&player_left + sizeof(Object));
        }
        if (ref_second.p->active_state != 0) {
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
        d -= (player_box->extent_y + obj_box->extent_y);
        if (d >= 0) {
            continue;
        }
        fi = (u16)obj_box->origin;
        if (ref_third.p->facing_side) {
            fi = -fi;
        }
        obj_x = fi + ref_third.p->pos_x;
        d = player_box->origin;
        if (ref_second.p->facing_side) {
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
        if (ref_second.p->bound_push_flag != 0) {
            ref_first.p = ref_second.p->other;
            ref_first.p->pos_x = ref_first.p->pos_x - d;
            ref_third.p->pos_x = ref_third.p->pos_x - d;
        } else {
            /* The second read of pos_x is wanted: with a local set from the first read and used here the unit builds 704 bytes and differs in 16 instruction slots. */
            ref_second.p->pos_x = ref_second.p->pos_x + d;
        }
    }
}
