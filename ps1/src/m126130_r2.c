/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801262fc(Attacker *a) {
    Object *p, *q;
    if (a->field_1b == 3)
        return;
    p = &player_left;
    q = p + 1;
    if ((a->field_1b & 1) == 0) {
        p = q;
        q = p - 1;
    }
    if (((a->field_50 >> table_8016f550[p->kind]) & 1) == 0 && p->field_ec >= 5 &&
        a->field_1c == 0 && p->field_db == 0) {
        a->field_af = 1;
        q->kind = table_8016f550[p->kind];
        a->field_84 = 1;
        a->field_8b = p->kind + 0x15;
    }
}

void func_801263d8(Attacker *a) {
    Object *p, *q;
    if (a->field_1b == 3)
        return;
    p = &player_left;
    q = p + 1;
    if ((a->field_1b & 1) == 0) {
        Object *t = p;
        p = q;
        q = t;
    }
    if ((a->field_50 & 0x100000) == 0 && a->field_1c == 0 && p->field_db == 0 &&
        (p->field_d4 & 1) == p->side && a->field_54 == 7 && p->field_ee >= 3) {
        a->field_af = 0xff;
        q->kind = 0x14;
        a->field_84 = 1;
        a->field_83 = 0;
        a->field_8b = 0x2a;
        if (p->kind == 4 || p->kind == 9 || p->kind == 0xd || p->kind == 0x12)
            a->field_8b = 0x2b;
    }
}
