/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Exact. The s16 local yi is assigned in both arms and again as the level
   in the later block, so the masked value stays alive next to the u8 copy yb. */
void check_hit_of_a_wide_box_against_b_boxes(Object *a, Object *b) {
    Box32 *box;
    u8 index;
    u16 xi;
    s16 yi;
    u8 yb;
    int lvl;

    if (b->countdown_4 != 0 || b->other_lock != 0 || b->field_163 != 0) {
        return;
    }
    if (a->field_49 == 0 || (b->field_295 == 0 && b->cooldown_timer == 0)) {
        lvl = b->field_69;
        if (a->frame->level < lvl) {
            return;
        }
    }
    index = a->frame->active;
    if (index == 0) {
        return;
    }
    box = &a->wide_boxes[index];
    xi = box->hit_flags & 0x7f;
    if (a->sound_side != 0) {
        yi = b->hit_box_index_b & 0x7f;
    } else {
        yi = b->hit_box_index_a & 0x7f;
    }
    yb = yi;
    if (yb == (u8)xi) {
        return;
    }
    if (a->mode_id != 8) {
        if (a->sound_side != 0) {
            yi = b->hit_count_b;
        } else {
            yi = b->hit_count_a;
        }
        if (box->y_limit < yi) {
            return;
        }
    }
    if (b->frame->box_a != 0) {
        record_box_a_overlap(a, b);
    }
    if (b->frame->box_b != 0) {
        record_box_b_overlap(a, b);
    }
    if (b->frame->box_c != 0) {
        record_box_c_overlap(a, b);
    }
    if (data_80188f30 != 0) {
        b->field_241 = 0;
        apply_hit_box_to_target(a, b);
        if (a->hit_skip_flag == 0) {
            apply_hit_damage_to_object(a, b, box);
            select_min_placement_and_test(a, b, box);
            add_table_score_to_player_on_hit(box, b);
        }
    }
}
