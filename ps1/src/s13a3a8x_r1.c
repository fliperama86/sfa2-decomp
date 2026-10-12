/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

extern Object *data_80188f20;
extern ObjectRef data_80188f24;
extern Object *data_80188f40;
extern u8 table_80177fd4[];
extern u8 table_80178274[];
extern u8 table_8017711c[];
extern u8 table_8017721c[];
extern u8 table_80178514[];
extern u8 table_801787b4[];

/* A second name for ref_third (the shared header declares it as a one-member struct, ObjectRef), here as a plain pointer, as in d1eb4c_r1.c; the alias is in symbols.ld. With ref_third.p written in its place the original's address load of the global is not reproduced (inferred: cse). */
extern Object *alloc_cursor_80190478;

/* Not in protos.h (inferred from the listing and the callers' use). */
void func_8013ae00(Object *a, Object *b, Tx *c);
void func_8013ad2c(Object *object);
void func_8013ad64(Object *object);

/* What it does (inferred, not an original name): applies one hit to the
   fighter a1. a0 is the attacker, a2 the attack's record. A damage value
   is built from tables (a base from the attacker's table, a random jitter
   of one table row, minus 0x10), scaled by a mode that comes from a0 or a1
   (x1, x2, x1.25, x1.5), capped at 0x7f and, for a1 in the state 0xff,
   cut. Let X be: a1->field_d8 is 0 and (a0->field_08 is 8 or
   a0->field_28a is 0). The cut is (dmg + 7) >> 3 (an eighth) when X and
   a2->field_14 is not 0, or when not X and a2->field_14 is 0; it is
   (dmg + 7) >> 2 (a quarter) when not X and a2->field_14 is not 0; and
   the function returns with no effect at all when a2->field_14 is 0 and
   (X or a1->health is 0). A second stage (defence tables of a1,
   a random term, a lookup in a damage table, a game option) turns it into
   the points taken from a1->health (a health value). A hit that cannot
   be computed (the first sum is negative, or the cut leaves 0) only
   removes 1 point, unless the option byte config->field_4c is not 0. On
   the main path a health below 0 after the points are taken is first set to
   -1 (with a1's field_163, field_15d and field_15e cleared), except that a1
   with field_d8 not 0 in the state 0xff hit by a record with field_14 equal
   to 0 is set to 0, which is no defeat. When the health is below 0 the
   function records the defeat: it calls func_8013a154 (when data_80188f34 is
   not 0) or func_8013ad64, writes a code into the attacking player's
   field_167 (only when that is 0) and config->field_8f, and calls
   func_80147000 (when a0->field_49 is not 0; otherwise when bit 0x40 of
   a2->field_08 is set, config->field_4e is 0 and a2->field_16 is not 0). The
   global pointers ref_first, ref_third and data_80188f24 are used as scratch
   by the original and written here in the same order, because their last
   values stay in memory (inferred). */
void func_8013a3a8(Object *a0, Object *a1, Box32 *a2) {
    unsigned dmg;
    s16 s1;
    s16 t;
    int u;
    int e;
    s16 s4;
    int s2;
    int x;
    u8 q;
    u8 *pp;
    u32 v6;
    u8 *bp;
    s16 s2h;
    s16 w;
    func_8013ae00(a0, a1, a2);
    if (a0->field_65) {
        ref_first.p = &player_right;
    } else {
        ref_first.p = &player_left;
    }
    data_80188f40 = ref_first.p;
    if (a0->field_49) {
        s1 = a2->field_1a;
    } else {
        s1 = a2->field_08;
    }
    {
        s1 = table_801777d4[ref_first.p->field_cf + ((s1 & 0x1f) << 5)];
        e = a2->field_0e;
        if ((s16)a0->health < 0x28) {
            e = a2->field_0f;
        }
        alloc_cursor_80190478 = data_80188f40;
    }
    /* The do-while wrapper reproduces the original's order of instructions: the compiler does not move an instruction across its ends. It emits no instruction of its own. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    /* Without it this function differs from the original in 27 instruction slots. */
    do {
        /* Stand-in, not recovered source: the local r, which is only a copy of a0, reproduces the original's order of instructions. Without it this function differs from the original in 27 instruction slots. */
        ObjectRef r;
        r.p = a0;
        data_80188f24 = r;
    } while (0);
    /* The do-while wrapper reproduces the original's order of instructions: the compiler does not move an instruction across its ends. It emits no instruction of its own. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    /* Without it this function differs from the original in 29 instruction slots. */
    do { ref_first.p = (Object *)(table_80177bd4 + (s16)e * 32); } while (0);
    alloc_cursor_80190478 = data_80188f24.p;
    e = *((u8 *)ref_first.p + (get_random_byte() & 0x1f));
    u = e - 0x10;
    t = s1 + u;
    if ((s16)t < 0) goto dec;
    dmg = t & 0xff;
    s1 = dmg;
    if (a0->field_08 == 0) {
        if (a1->field_159) {
            if (a0->field_29c == 0) {
            if (a1->field_29c == 0) {
            } else if (a1->field_29c == 1) {
                s1 = dmg << 1;
            } else if (a1->field_29c == 2) {
                s1 = dmg + s1 / 4;
            } else if (a1->field_29c == 3) {
                s1 = dmg + (dmg >> 1);
            }
            } else if (!(a1->frame->field_0c & 0x80)) {
            if (a0->field_29c == 1) {
                s1 = dmg << 1;
            } else if (a0->field_29c == 2) {
                s1 = dmg + s1 / 4;
            } else if (a0->field_29c == 3) {
                s1 = dmg + (dmg >> 1);
            }
            } else {
            if (a1->field_29c == 0) {
            } else if (a1->field_29c == 1) {
                s1 = dmg << 1;
            } else if (a1->field_29c == 2) {
                s1 = dmg + s1 / 4;
            } else if (a1->field_29c == 3) {
                s1 = dmg + (dmg >> 1);
            }
            }
        } else {
            if (a1->field_29c == 0) {
            } else if (a1->field_29c == 1) {
                s1 = dmg << 1;
            } else if (a1->field_29c == 2) {
                s1 = dmg + s1 / 4;
            } else if (a1->field_29c == 3) {
                s1 = dmg + (dmg >> 1);
            }
        }
    }
    if ((s16)s1 >= 0x80) {
        s1 = 0x7f;
    }
    if (a1->field_61 == 0xff) {
        if (a0->field_08 != 8) {
            if (a0->field_28a == 0) {
                if (a1->field_d8 == 0) goto f;
                if (a2->field_14 != 0) {
                    x = (s16)(s1 + 7) >> 2;
                } else {
                    if ((s16)a1->health == 0) return;
                    x = (s16)(s1 + 7) >> 3;
                }
            } else {
                if (a2->field_14 != 0) {
                    x = (s16)(s1 + 7) >> 2;
                } else {
                    if ((s16)a1->health == 0) return;
                    x = (s16)(s1 + 7) >> 3;
                }
            }
        } else {
            if (a1->field_d8 == 0) goto f;
            if (a2->field_14 != 0) goto q;
            if ((s16)a1->health == 0) return;
            x = (s16)(s1 + 7) >> 3;
        }
        goto g;
f:
        if (a2->field_14 == 0) return;
        x = (s16)(s1 + 7) >> 3;
        goto g;
q:
        x = (s16)(s1 + 7) >> 2;
g:
        if (x == 0) goto dec;
        s1 = x;
    } else {
        if (a2->field_15 == 5 || (a2->field_08 & 0x40)) {
            a1->field_260 = 0xff;
        }
        {
            e = a2->field_0c;
            if (a0->field_49) {
                e = 0;
                a1->field_15e = 0;
            }
            a1->field_15d = 0xd2;
            a1->field_15e = e + a1->field_15e;
        }
    }
    s1 = (u8)s1;
    if (!(a1->field_15e < a1->field_162)) {
        func_8013ad2c(a1);
    }
    if (a1->field_cd) {
        ref_first.p = (Object *)table_80178274;
    } else {
        ref_first.p = (Object *)table_80177fd4;
    }
    e = 0x1f;
    {
        /* bp keeps the table pointer in ref_first across the reassignment of ref_first below; with (u8 *)ref_first.p + ... written in pp's line this function differs from the original in 4 instruction slots. */
        bp = (u8 *)ref_first.p;
        /* pp holds the address of the defence byte that is read below; with *(bp + ...) at the read instead this function differs from the original in 64 instruction slots. */
        pp = bp + (a1->field_cf + ((a1->kind & 0x1f) << 5));
        /* The do-while wrapper reproduces the original's order of instructions: the compiler does not move an instruction across its ends. It emits no instruction of its own. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
        /* Without it this function differs from the original in 28 instruction slots. */
        do { v6 = a1->field_6a; } while (0);
        w = *pp;
        s2 = ((a1->kind & 0x1f) << 5);
        /* Stand-in, not recovered source: one stand-in at 3 places, the term that is ORed to 0x1f below, which is 0 for every value of s2, reproduces the original's choice of registers. Without it at all 3 places this function differs from the original in 55 instruction slots. */
        if ((u32)v6 < (0x1f | (int)(((u32)s2 << 24) & 0x100))) {
            e = 0;
            if (a1->field_295) {
                e = 0x80;
                if (a2->field_1c) {
                    e = 0xe0;
                    w = 0;
                } else {
                    if (a2->field_08 & 0x80) {
                        if (a2->field_08 & 0x40) {
                            e = 0xc0;
                        } else {
                            e = 0xa0;
                        }
                    }
                }
            } else {
                if (a2->field_08 & 0x80) {
                    if (a2->field_08 & 0x40) {
                        e = 0x40;
                    } else {
                        e = 0x20;
                    }
                }
            }
            if ((s16)e == 0x80) {
                e = (a2->field_17 != 0) << 5;
                if (a2->field_17 != 1) e = 0x40;
                ref_first.p = (Object *)table_8017721c;
            } else {
                ref_first.p = (Object *)table_8017711c;
            }
            e = *((u8 *)ref_first.p + (a1->field_6a + (s16)e));
        }
        s4 = w + e;
    }
    {
        w = s4;
        /* The second place of the stand-in above. The figure is the one above: the term that is ORed to a2->field_1b, taken out at all 3 places. */
        if (a2->field_1b | (int)(((u32)s2 << 24) & 0x100)) {
            a1->field_28f = a2->field_1b;
        }
        if ((s16)a1->health < 0x28) {
            s2 = table_80178514[s2 + (get_random_byte() & 0x1f)];
            /* Stand-in, not recovered source: the local s2h, which is only a copy of s2 narrowed to s16, reproduces the original's choice of registers. Without it this function differs from the original in 1 instruction slot. */
            s2h = s2;
            w = s2h + s4;
        }
        if (w > 0x20) w = 0x20;
        /* The third place of the stand-in above. The figure is the one above: the term that is added to the index, taken out at all 3 places. */
        w = table_801787b4[((u8)w << 7) + (s16)s1 + (int)(((u32)s2 << 24) & 0x100)];
        s2 = ((u8 *)table_8017984c)[w + (table_80179834[a1->kind] >> 7)];
        w = w + s2;
    }
    {
        int m = game_state.config->field_12;
        if (m == 0) {
            w = w - (w >> 2);
        } else if (m == 1) {
            /* Nothing changes for 1; the original has the test for it. Without this arm this function differs from the original in 14 instruction slots. */
        } else if (m == 2) {
            w = w + (w >> 2);
        } else if (m == 3) {
            w = w + (w >> 1);
        }
    }
    if (w >= 0x80) w = 0x7f;
    if (*(u16 *)&game_state.config->field_4c == 0) {
        a1->health = a1->health - w;
        if ((s16)a1->health >= 0) return;
        if (a1->field_d8 != 0 && a1->field_61 == 0xff && a2->field_14 == 0) {
            a1->health = 0;
        } else {
            *(s16 *)&a1->health = -1;
            a1->field_163 = 0;
            a1->field_15d = 0;
            a1->field_15e = 0;
        }
    }
    goto chk;
dec:
    if (game_state.config->field_4c == 0) {
        a1->health = a1->health - 1;
    }
chk:
    if ((s16)a1->health < 0) {
        if (data_80188f34) {
            func_8013a154(a0, a1, a2);
        } else {
            func_8013ad64(a1);
        }
        if (a0->field_65) {
            ref_first.p = &player_right;
        } else {
            ref_first.p = &player_left;
        }
        if (ref_first.p->field_167 == 0) {
            e = ref_first.p->kind + 0x10;
            if (a0->field_49 == 0) {
                e = a2->field_15;
                if (data_80188f34) {
                    if (!(a2->field_08 & 0x40)) {
                        e = 7;
                    } else {
                        e = 0xb;
                    }
                }
            }
            ref_first.p->field_167 = e;
            game_state.config->field_8f = ref_first.p->kind;
        }
        if (a0->field_49) {
            data_80188f20 = ref_other.p;
            ref_first.p->field_255 = 6;
            ref_first.p->field_c6 = 0;
            game_state.config->field_6b = 0;
            func_80147000(a0);
            ref_other.p = data_80188f20;
        } else if (a2->field_08 & 0x40) {
            if (game_state.config->field_4e != 0) return;
            if (a2->field_16 == 0) return;
            data_80188f20 = ref_other.p;
            if (a0->field_0b != 0) {
                game_state.config->field_6b = a2->field_16 - 1;
                func_80147000(a0);
                ref_other.p = data_80188f20;
            } else {
                e = a2->field_16;
                q = e;
                if (q == 2) {
                    e = e + 1;
                } else if (q == 3) {
                    e = e - 1;
                }
                e = e - 1;
                game_state.config->field_6b = e;
                func_80147000(a0);
                ref_other.p = data_80188f20;
            }
        }
    }
}

