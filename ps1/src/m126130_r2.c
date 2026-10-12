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
    if (((a->bit_flags >> table_8016f550[p->kind]) & 1) == 0 && p->hit_count >= 5 &&
        a->field_1c == 0 && p->db_flag == 0) {
        a->mode_marker = 1;
        q->kind = table_8016f550[p->kind];
        a->phase = 1;
        a->kind_code = p->kind + 0x15;
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
    if ((a->bit_flags & 0x100000) == 0 && a->field_1c == 0 && p->db_flag == 0 &&
        (p->field_d4 & 1) == p->side && a->field_54 == 7 && p->repeat_count >= 3) {
        a->mode_marker = 0xff;
        q->kind = 0x14;
        a->phase = 1;
        a->special_off_flag = 0;
        a->kind_code = 0x2a;
        if (p->kind == 4 || p->kind == 9 || p->kind == 0xd || p->kind == 0x12)
            a->kind_code = 0x2b;
    }
}
