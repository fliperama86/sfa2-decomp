/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* r is deliberately left unassigned before the first loop pass, as in the original. */
void func_80156898(void *a, Object *object) {
    ScoreRec *r;
    ScoreRec *p;
    Object *other;
    u32 score;
    int i;
    s16 j;
    int n;

    p = table_8016e604;
    score = object->field_e4;
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
            r->field_08 = p->field_08;
            r->field_09 = p->field_09;
            r->field_0a = p->field_0a;
            r->field_0b = p->field_0b;
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
        r->field_08 = object->field_118;
        r->field_09 = object->field_119;
        r->field_0a = object->field_11a;
        r->field_0c = object->field_124;
    }
    other = &player_left;
    if (object->side == 0) {
        other++;
    }
    if (other->score_rank != 0 && other->score_rank >= j && other->score_rank != 6) {
        other->score_rank++;
    }
}

void func_80156a20(void *a, Object *object) {
    ScoreRec *r;
    ScoreRec *p;
    Object *other;
    u32 score;
    u8 c;
    int i;
    int n;
    s16 j;

    p = table_8016e654;
    c = object->field_124;
    score = object->field_e0;
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
            r->field_08 = p->field_08;
            r->field_09 = p->field_09;
            r->field_0a = p->field_0a;
            r->field_0b = p->field_0b;
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
        r->field_08 = object->field_118;
        r->field_09 = object->field_119;
        r->field_0a = object->field_11a;
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
