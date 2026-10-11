/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

void func_80135c0c(u8 *a, u8 *b, u8 index);

/* What it does (inferred, not an original name): updates the strips of the two player objects. Mode 1 of data_80190474: the object that game_state.field_78 points at (when not null) has its field_65 select player_left (0) or the next object (not 0); its field_167 is stored in that player's byte table at the index field_ce - 1, the player is passed to func_80120554, and the two strips of that index are passed to func_80135c0c with a code made from field_167 (shifted left by two, bit 1 set when bit 7 was set). Mode 2: for each player and each of the four table entries that holds 8, the two strips of that index are passed with code 0x20. */
void func_80135a10(void) {
    Strip1c *b;
    Strip1c *a;
    u8 v;
    u8 i;
    Object *o;
    Object *l = &player_left;
    Object *r = &player_left + 1;
    if (data_80190474 == 1) {
        o = game_state.field_78;
        if (o != 0) {
            v = o->field_167;
            i = o->field_ce - 1;
            if (o->field_65 == 0) {
                l->bytes_d0[i] = v;
                /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
                do {
                    /* Stand-in, not recovered source: a second do-while wrapper, nested inside the first around the same two statements, reproduces the original's choice of registers. Without it this function differs from the original in 22 instruction slots. */
                    do {
                        a = &strips[0][i];
                        b = &strips[0][i + 4];
                    } while (0);
                } while (0);
                func_80120554(l, 0, 0x34c);
            } else {
                r->bytes_d0[i] = v;
                /* Stand-in, not recovered source: a do-while wrapper around the two statements of this arm reproduces the original's choice of registers. Without it this function differs from the original in 22 instruction slots. */
                do {
                    a = &strips[0][i + 8];
                    b = &strips[0][i + 12];
                } while (0);
                func_80120554(r, 1, 0x34c);
            }
            if (v & 0x80) {
                v = ((v & 0x7f) << 2) | 2;
            } else {
                v = v << 2;
            }
            func_80135c0c((u8 *)a, (u8 *)b, v);
        }
    } else if (data_80190474 == 2) {
        v = 0x20;
        i = 0;
        do {
            if (l->bytes_d0[i] == 8) {
                func_80135c0c((u8 *)&strips[0][i], (u8 *)&strips[0][i + 4], v);
            }
            i++;
        } while (i < 4);
        i = 0;
        do {
            if (r->bytes_d0[i] == 8) {
                func_80135c0c((u8 *)&strips[0][i + 8], (u8 *)&strips[0][i + 12], v);
            }
            i++;
        } while (i < 4);
    }
}
