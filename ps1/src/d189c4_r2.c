/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

void func_8011cbb8(Slab172 *p);
void draw_object_tile_grid_prims(Object *object, FrameRecord *frames);

/* What it does (inferred, not an original name): sets field_09 of the two
   fighters (player_left, player_right). When either has a non-zero field_73,
   the one with the larger compare_value gets 1 and the other 0; with equal
   values the right one gets it if its field_73 is 1, else the left one.
   Otherwise the one with the smaller frame value_a gets 1 and the other 0,
   and with equal values nothing is stored. Then, for each fighter, the four
   part blocks that are present and active get field_09 + 4 and go to
   func_8011bc84, and func_8011cbb8 and draw_object_tile_grid_prims run when field_00 and
   field_01 are set. */
void func_8011a880(void) {
    Fighter *left = (Fighter *)&player_left;
    /* The second player is reached from the first: with &player_right 36 instruction lines differ. */
    Fighter *right = left + 1;
    Block172 *p;

    if (left->field_73 != 0 || right->field_73 != 0) {
        left->part_mode = 0;
        right->part_mode = 0;
        /* Plainer spellings differ: one condition with || in 7 instruction
           lines, the >= form with one store per side in 13. */
        if (left->compare_value >= right->compare_value) {
            if (left->compare_value == right->compare_value && right->field_73 == 1) {
                right->part_mode = 1;
            } else {
                left->part_mode = 1;
            }
        } else {
            right->part_mode = 1;
        }
    } else {
        /* The original reads through both frame pointers here without a null test. */
        if (left->frame->value_a > right->frame->value_a) {
            left->part_mode = 0;
            right->part_mode = 1;
        } else if (right->frame->value_a > left->frame->value_a) {
            left->part_mode = 1;
            right->part_mode = 0;
        }
    }
    p = left->part_28;
    if (p != 0 && p->enabled_flag != 0) {
        p->field_09 = left->part_mode + 4;
        func_8011bc84((Slab172 *)p);
    }
    p = left->part_2c;
    if (p != 0 && p->enabled_flag != 0) {
        p->field_09 = left->part_mode + 4;
        func_8011bc84((Slab172 *)p);
    }
    if (left->field_00 != 0 && left->parts_active_flag != 0) {
        func_8011cbb8((Slab172 *)left);
    }
    p = left->part_30;
    if (p != 0 && p->enabled_flag != 0) {
        p->field_09 = left->part_mode + 4;
        func_8011bc84((Slab172 *)p);
    }
    p = left->part_34;
    if (p != 0 && p->enabled_flag != 0) {
        p->field_09 = left->part_mode + 4;
        func_8011bc84((Slab172 *)p);
    }
    if (left->field_00 != 0 && left->parts_active_flag != 0) {
        draw_object_tile_grid_prims((Object *)left, frames_left);
    }
    p = right->part_28;
    if (p != 0 && p->enabled_flag != 0) {
        p->field_09 = right->part_mode + 4;
        func_8011bc84((Slab172 *)p);
    }
    p = right->part_2c;
    if (p != 0 && p->enabled_flag != 0) {
        p->field_09 = right->part_mode + 4;
        func_8011bc84((Slab172 *)p);
    }
    if (right->field_00 != 0 && right->parts_active_flag != 0) {
        func_8011cbb8((Slab172 *)right);
    }
    p = right->part_30;
    if (p != 0 && p->enabled_flag != 0) {
        p->field_09 = right->part_mode + 4;
        func_8011bc84((Slab172 *)p);
    }
    p = right->part_34;
    if (p != 0 && p->enabled_flag != 0) {
        p->field_09 = right->part_mode + 4;
        func_8011bc84((Slab172 *)p);
    }
    if (right->field_00 != 0 && right->parts_active_flag != 0) {
        draw_object_tile_grid_prims((Object *)right, frames_right);
    }
}
