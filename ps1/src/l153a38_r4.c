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
        if (score < p->field_00) {
            break;
        }
        if ((s16)i-- != 4) {
            r->field_00 = p->field_00;
            r->field_04 = p->field_04;
            r->field_05 = p->field_05;
            r->field_06 = p->field_06;
            r->field_07 = p->field_07;
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
    object->field_120 = j;
    if (j < 6) {
        r->field_04 = object->field_11c;
        r->field_05 = object->field_11d;
        r->field_06 = object->field_11e;
        r->field_07 = object->field_11f;
        r->field_00 = score;
        r->field_08 = object->field_118;
        r->field_09 = object->field_119;
        r->field_0a = object->field_11a;
        r->field_0c = object->field_124;
    }
    other = &player_left;
    if (object->side == 0) {
        other++;
    }
    if (other->field_120 != 0 && other->field_120 >= j && other->field_120 != 6) {
        other->field_120++;
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
        if (p->field_0c == c && score < p->field_00) {
            break;
        }
        if ((s16)i-- != 4) {
            r->field_00 = p->field_00;
            r->field_04 = p->field_04;
            r->field_05 = p->field_05;
            r->field_06 = p->field_06;
            r->field_07 = p->field_07;
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
        r->field_04 = object->field_11c;
        r->field_05 = object->field_11d;
        r->field_06 = object->field_11e;
        r->field_07 = object->field_11f;
        r->field_00 = score;
        r->field_08 = object->field_118;
        r->field_09 = object->field_119;
        r->field_0a = object->field_11a;
        r->field_0c = c;
    }
    other = &player_left;
    if (object->side == 0) {
        other++;
    }
    if (other->field_120 != 0 && other->field_120 >= j && other->field_120 != 6) {
        other->field_120++;
    }
}
