/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* What it does (inferred, not an original name): computes a byte value in
   damage of the object that ref_other points at, and returns it. With a
   == -1 it returns 0 at once. Otherwise it starts from a table byte chosen
   by the object's table_column and the low five bits of `a` (32 per step),
   adds 0xf0 and a second table byte chosen by a random number (low five
   bits) combined with b << 5, clears the result when bit 7 is set, selects
   a row of table_8017984c through table_80179834[object->kind] (the row's
   address is stored in game_state.cursor), clamps the value to 0x7f when it
   is 0x80 or more (bit 7 was cleared just before, so the clamp is not
   reached; inferred), adds that row's entry for the value, calls
   func_80140ec0 (which calls through table_8017ac34, a handler that may
   change damage; read from the original's listing, not tested), and
   returns damage halved when field_15b is 0, but at least 1. */
int func_80140b5c(Object *object, s16 a, int b) {
    u8 v;
    unsigned w;
    s16 idx;
    int s;
    int t;

    if (a != -1) {
        ref_other.p->damage = table_801777d4[object->table_column + ((a & 0x1f) << 5)];
        s = b << 5;
        /* The call of get_random_byte goes through a type that returns int although the callee returns a byte: the original does not narrow the result at the call. Written plainly, this function differs from the original in 11 instruction slots. */
        /* Not portable C: a call through a type that is not compatible with the callee's definition. It relies on the PS1's calling convention, under which the callee leaves its byte zero-extended in the whole result register. */
        /* The original keeps the value of the function's last part in a0 and masks it to a byte at each of its uses. That value is the variable w, which first takes this call's result. With the routine's declared byte result the compiler knows that w never exceeds a byte: it drops the masks and uses v0 (372 bytes). The original's caller did not know the width of the result (inferred: no prototype in scope there, or one that returns int). Only the low five bits of the result are used, so the function computes the same with the plain call. */
        w = ((int (*)(void))get_random_byte)();
        /* The OR's value and, further down, the row index share the local t. With the row index written at the table (t used once) this function differs from the original in 8 instruction slots: the value then gets its register together with the shift behind it. */
        t = (w & 0x1f) | s;
        idx = t;
        ref_other.p->damage += 0xf0 + table_80177bd4[idx];
        if (ref_other.p->damage & 0x80) {
            ref_other.p->damage = 0;
        }
        t = table_80179834[object->kind];
        game_state.cursor = (u32 *)table_8017984c[t];
        v = ref_other.p->damage;
        if (v >= 0x80) {
            v = 0x7f;
        }
        ref_other.p->damage = v + ((u8 *)game_state.cursor)[v];
        func_80140ec0();
        w = ref_other.p->damage;
        if (ref_other.p->field_15b == 0) {
            w >>= 1;
        }
        if ((u8)w == 0) {
            w = 1;
        }
        v = (u8)w;
        return v;
    }
    return 0;
}
