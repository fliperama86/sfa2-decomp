/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

void func_8011cbb8(Slab172 *p);
void func_80151324(Object *object, FrameRecord *frames);

/* What it does (inferred, not an original name): sets field_09 of the two
   fighters (player_left, player_right). When either has a non-zero field_73,
   the one with the larger field_17a gets 1 and the other 0; with equal
   values the right one gets it if its field_73 is 1, else the left one.
   Otherwise the one with the smaller frame field_04 gets 1 and the other 0,
   and with equal values nothing is stored. Then, for each fighter, the four
   part blocks that are present and active get field_09 + 4 and go to
   func_8011bc84, and func_8011cbb8 and func_80151324 run when field_00 and
   field_01 are set. */
void func_8011a880(void) {
    Fighter *left = (Fighter *)&player_left;
    /* The second player is reached from the first: with &player_right 36 instruction lines differ. */
    Fighter *right = left + 1;
    Block172 *p;

    if (left->field_73 != 0 || right->field_73 != 0) {
        left->field_09 = 0;
        right->field_09 = 0;
        /* Plainer spellings differ: one condition with || in 7 instruction
           lines, the >= form with one store per side in 13. */
        if (left->field_17a >= right->field_17a) {
            if (left->field_17a == right->field_17a && right->field_73 == 1) {
                right->field_09 = 1;
            } else {
                left->field_09 = 1;
            }
        } else {
            right->field_09 = 1;
        }
    } else {
        /* The original reads through both frame pointers here without a null test. */
        if (left->frame->field_04 > right->frame->field_04) {
            left->field_09 = 0;
            right->field_09 = 1;
        } else if (right->frame->field_04 > left->frame->field_04) {
            left->field_09 = 1;
            right->field_09 = 0;
        }
    }
    p = left->part_28;
    if (p != 0 && p->field_01 != 0) {
        p->field_09 = left->field_09 + 4;
        func_8011bc84((Slab172 *)p);
    }
    p = left->part_2c;
    if (p != 0 && p->field_01 != 0) {
        p->field_09 = left->field_09 + 4;
        func_8011bc84((Slab172 *)p);
    }
    if (left->field_00 != 0 && left->field_01 != 0) {
        func_8011cbb8((Slab172 *)left);
    }
    p = left->part_30;
    if (p != 0 && p->field_01 != 0) {
        p->field_09 = left->field_09 + 4;
        func_8011bc84((Slab172 *)p);
    }
    p = left->part_34;
    if (p != 0 && p->field_01 != 0) {
        p->field_09 = left->field_09 + 4;
        func_8011bc84((Slab172 *)p);
    }
    if (left->field_00 != 0 && left->field_01 != 0) {
        func_80151324((Object *)left, frames_left);
    }
    p = right->part_28;
    if (p != 0 && p->field_01 != 0) {
        p->field_09 = right->field_09 + 4;
        func_8011bc84((Slab172 *)p);
    }
    p = right->part_2c;
    if (p != 0 && p->field_01 != 0) {
        p->field_09 = right->field_09 + 4;
        func_8011bc84((Slab172 *)p);
    }
    if (right->field_00 != 0 && right->field_01 != 0) {
        func_8011cbb8((Slab172 *)right);
    }
    p = right->part_30;
    if (p != 0 && p->field_01 != 0) {
        p->field_09 = right->field_09 + 4;
        func_8011bc84((Slab172 *)p);
    }
    p = right->part_34;
    if (p != 0 && p->field_01 != 0) {
        p->field_09 = right->field_09 + 4;
        func_8011bc84((Slab172 *)p);
    }
    if (right->field_00 != 0 && right->field_01 != 0) {
        func_80151324((Object *)right, frames_right);
    }
}
