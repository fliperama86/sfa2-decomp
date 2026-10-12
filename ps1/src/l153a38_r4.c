/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* r is deliberately left unassigned before the first loop pass, as in the original. */
void insert_score_in_table(void *a, Object *object) {
    ScoreRec *r;
    ScoreRec *p;
    Object *other;
    u32 score;
    int i;
    s16 j;
    int n;

    p = table_8016e604;
    score = object->score_2;
    i = 4;
    for (n = 0; n < 5; n++) {
        if (score < p->score) {
            break;
        }
        if ((s16)i-- != 4) {
            r->score = p->score;
            r->name_char_0 = p->name_char_0;
            r->name_char_1 = p->name_char_1;
            r->name_char_2 = p->name_char_2;
            r->name_char_3 = p->name_char_3;
            r->extra_byte_0 = p->extra_byte_0;
            r->extra_byte_1 = p->extra_byte_1;
            r->extra_byte_2 = p->extra_byte_2;
            r->extra_byte_3 = p->extra_byte_3;
            r->field_0c = p->field_0c;
        }
        r = p;
        p = r - 1;
    }
    j = i + 2;
    object->score_rank = j;
    if (j < 6) {
        r->name_char_0 = object->name_char_0;
        r->name_char_1 = object->name_char_1;
        r->name_char_2 = object->name_char_2;
        r->name_char_3 = object->name_char_3;
        r->score = score;
        r->extra_byte_0 = object->saved_side;
        r->extra_byte_1 = object->select_value_1;
        r->extra_byte_2 = object->select_value_2;
        r->field_0c = object->score_sort_key;
    }
    other = &player_left;
    if (object->side == 0) {
        other++;
    }
    if (other->score_rank != 0 && other->score_rank >= j && other->score_rank != 6) {
        other->score_rank++;
    }
}

void insert_score_into_ranking_table(void *a, Object *object) {
    ScoreRec *r;
    ScoreRec *p;
    Object *other;
    u32 score;
    u8 c;
    int i;
    int n;
    s16 j;

    p = table_8016e654;
    c = object->score_sort_key;
    score = object->max_score;
    i = 4;
    for (n = 0; n < 5; n++) {
        if (c < p->field_0c) {
            break;
        }
        if (p->field_0c == c && score < p->score) {
            break;
        }
        if ((s16)i-- != 4) {
            r->score = p->score;
            r->name_char_0 = p->name_char_0;
            r->name_char_1 = p->name_char_1;
            r->name_char_2 = p->name_char_2;
            r->name_char_3 = p->name_char_3;
            r->extra_byte_0 = p->extra_byte_0;
            r->extra_byte_1 = p->extra_byte_1;
            r->extra_byte_2 = p->extra_byte_2;
            r->extra_byte_3 = p->extra_byte_3;
            r->field_0c = p->field_0c;
        }
        r = p;
        p = r - 1;
    }
    j = i + 2;
    object->field_121 = j;
    if (j < 6) {
        r->name_char_0 = object->name_char_0;
        r->name_char_1 = object->name_char_1;
        r->name_char_2 = object->name_char_2;
        r->name_char_3 = object->name_char_3;
        r->score = score;
        r->extra_byte_0 = object->saved_side;
        r->extra_byte_1 = object->select_value_1;
        r->extra_byte_2 = object->select_value_2;
        r->field_0c = c;
    }
    other = &player_left;
    if (object->side == 0) {
        other++;
    }
    if (other->score_rank != 0 && other->score_rank >= j && other->score_rank != 6) {
        other->score_rank++;
    }
}
