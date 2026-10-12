/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* What it does (inferred, not an original name): one step of a jump or fall
   toward a target height, ground_y. func_80130184 moves the object by its
   velocity words and reports whether pos_y has reached ground_y. When it
   has, the object lands: pos_x_frac, active_state and active_flag_3 are cleared, the
   state byte state_3 advances, pos_y is set to the target, a sound is
   played and sequence 0x36 starts. Otherwise the remaining distance
   (target minus pos_y, as a signed halfword) picks a sequence: 0x3b below
   0x11, 0x3a below 0x21, and for a larger distance the current sequence
   simply steps on (update_sequence). */
void func_801b339c_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s16 d;
    int seq;
    int res;
    Object *p;

    res = (u8)func_80130184(o);
    /* Stand-in, not recovered source: the local p, a second name of the object that is set here and again behind the sound call, with a term added to its first value that is 0 for every value of res, reproduces the original's choice of registers. Without it this function differs from the original in 5 instruction slots. */
    /* The original copies the object into the first argument register behind the first call (in the delay slot of the test) and again behind the sound call, and the shared call at the end has an empty delay slot. With the object passed directly, or with `p = o;` here, the build copies it once, in the delay slot of the shared call, and is one instruction shorter (176 bytes): the compiler sees that p is the object before the registers are given. The term hides that until its pass that joins instructions, and leaves no instruction. It is an unsigned shift of a 32-bit value whose low 24 bits are then clear, masked with 0x100, so it is 0 on every compiler, and it is added to the pointer as a count of bytes. This reading of the compiler is inferred. */
    p = (Object *)((u8 *)o + (((u32)res << 24) & 0x100));
    if (res == 0) {
        o->pos_x_frac = 0;
        o->active_state = 0;
        o->active_flag_3 = 0;
        o->state_3++;
        o->pos_y = obj->ground_y;
        play_side_sound_at(o, o->side, 0x1b);
        p = o;
        seq = 0x36;
    } else {
        d = obj->ground_y - (u16)o->pos_y;
        if (d < 0x11) {
            seq = 0x3b;
        } else if (d < 0x21) {
            seq = 0x3a;
        } else {
            update_sequence(o);
            return;
        }
    }
    func_801307e0(p, seq);
}
