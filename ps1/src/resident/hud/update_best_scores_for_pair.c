/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void update_best_scores_for_pair(Entity *entity, Object *a, Object *b) {
    int t;
    a->best_score = a->score;
    b->best_score = b->score;
    if (entity->field_1b == 0) {
        update_object_scores_from_source(entity, &player_left);
        update_object_scores_from_source(entity, &player_left + 1);
    } else if (entity->field_1b == 3) {
        t = a->score;
        a->score = a->score_2;
        if (a->max_score < t) {
            a->max_score = t;
        }
        t = b->score;
        b->field_11b = entity->field_40;
        b->score = b->score_2;
        if (b->max_score < t) {
            b->max_score = t;
        }
    } else {
        if (a->mode_flag_2 != 0) {
            update_object_scores_from_source(entity, b);
        } else {
            if (a->count < 0x63) {
                a->count++;
            }
            t = subtract_bcd(a->best_score, a->score_2);
            a->best_score = t;
            a->score_2 = a->score;
        }
    }
    update_object_scores_from_source(entity, b);
}
