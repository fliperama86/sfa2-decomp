/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* Inferred declarations; not original ones. */
int func_8016a7e4(unsigned mask);
void func_80164140(int a, int b, int c, int d, int e, int f, short g, short h);
void func_80164ef0(int a, int b, unsigned short c, unsigned short d);
void func_80168f50(short a, short b, unsigned char c, short d);
void func_80164a78(int a, int b, int c);
void func_8016452c(int a);

/* What it does (inferred, not an original name): plays a list of sound
   commands. The list is a run of 8-byte entries found through
   table_8016e6e0[song] and start; the walk goes on while bit 23 of the second
   word is set. The command is bits 12 to 15 of the first word: 0 starts a note
   on a channel (func_80164140), unless the channel holds a higher priority;
   1 sets the volumes from a pan and makes three calls; 9 and 11 call with the
   low 12 bits (11 stops a channel, or all with 0xfff). Song 0x80 stops all. */
/* Figures below come from the project's check, which counts 2 instruction lines for the
   address of this unit's jump table (in the exact form too); each figure includes them. */
void func_80120604(unsigned song, int alt, unsigned short start, int pos) {
    /* A pointer to a pair of words; with a plain unsigned pointer and cmd[0], cmd[1] this function
       differs from the original in 103 instruction lines. */
    unsigned (*cmd)[2];
    /* chan is u16 (as an int it differs in 62 lines) and is copied once into chan_idx (using chan
       directly differs in 5 lines; an int chan_idx in 17). */
    u16 chan;
    u16 chan_idx;
    int prio;
    int step;
    int raw_pan;
    short pan;
    int vol_max;
    unsigned left;
    unsigned right;
    unsigned note;
    u8 *base;
    s8 *song_data;
    u16 tone;
    /* As an int, vab differs in 40 instruction lines; as an unsigned, tone in 4. */
    s16 vab;
    u8 prog;
    u8 fine;

    base = data_80190a44;
    if (song == 0x80) {
        func_80164bbc(0);
        return;
    }
    song_data = data_80190a44 + song;
    /* The local holds a constant: the original keeps this value in a register of its own. The name is inferred. */
    /* With 0x7f written at its uses this function differs from the original in 64 instruction lines. */
    vol_max = 0x7f;
    cmd = (unsigned (*)[2])table_8016e6e0[song] + start;
    do {
        switch (((*cmd)[0] >> 12) & 0xf) {
        case 0:
            chan = ((*cmd)[1] >> 7) & 0x1f;
            if (alt != 0) {
                chan += 4;
            }
            prio = ((*cmd)[1] >> 16) & 0x7f;
            chan_idx = chan;
            /* The channel's priority is the byte at base + 0x748c + channel (data_80197ed0). Written as
               that symbol, the function differs in 80 lines; the same for table_80197ef8 below (73). */
            if (prio < (base + chan_idx)[0x748c]) {
                if (func_8016a7e4(1 << chan_idx) == 2) {
                    break;
                }
            }
            /* The field is read again; storing prio differs in 98 instruction lines. */
            (base + chan_idx)[0x748c] = ((*cmd)[1] >> 16) & 0x7f;
            /* The local holds a constant: the original keeps this value in a register of its own. The name is inferred. */
            /* With 0 written at its use this function differs from the original in 25 instruction lines. */
            fine = 0;
            vab = song_data[0];
            /* Bytes 2 and 3 of the first word are read as bytes everywhere; as shifts of the word,
               byte 3 differs in 20 instruction lines and byte 2 in 48. */
            prog = ((u8 *)cmd)[3];
            tone = ((*cmd)[1] >> 12) & 0xf;
            left = (*cmd)[1] & 0x7f;
            right = left;
            /* Without the local note, the expression in the call differs in 37 lines (prog: 10). */
            note = ((*cmd)[1] >> 24) & 0x7f;
            if (((u8 *)cmd)[2] == 0xff) {
                /* The step is taken as a short and stored in pos after it (pos += 0xc0 first: 39 lines;
                   without the cast: 46 lines). */
                step = (short)(pos + 0xc0);
                pos += 0xc0;
                if (step == 0) {
                    raw_pan = 0;
                } else {
                    raw_pan = step / 6;
                }
            } else {
                raw_pan = ((u8 *)cmd)[2];
            }
            pan = raw_pan;
            /* The volumes are used as shorts; without the casts this differs in 38 instruction lines. */
            if (pan < 0x40) {
                left = (unsigned)(pan * (short)left) >> 6;
            } else {
                right = (unsigned)((vol_max - pan) * (short)right) >> 6;
            }
            func_80164140(chan, vab, prog, tone, note, fine, right, left);
            break;
        case 1:
            step = (short)(pos + 0xc0);
            pos += 0xc0;
            if (step == 0) {
                raw_pan = 0;
            } else {
                raw_pan = step / 6;
            }
            left = 0x7f;
            pan = raw_pan;
            right = 0x7f;
            if (pan < 0x40) {
                left = (unsigned)(pan * vol_max) >> 6;
            } else {
                right = (unsigned)((vol_max - pan) * vol_max) >> 6;
            }
            /* The low 12 bits are taken as % 0x1000 in every place; as & 0xfff the function differs in 4 lines. */
            if (song < 2) {
                /* table_80197ef8[song] = start, as base + 0x74b4. */
                ((u16 *)(base + 0x74b4))[song] = start;
            }
            /* Called as the definition (without this cast) it differs in 17 instruction lines.
               The definition (s160fbc_r10_b.c) takes its last two parameters as unsigned short.
               The original's code sign-extends them here before the call, so the call is made
               through a type that takes them as short. */
            ((void (*)(int, int, short, short))func_80164ef0)(song_data[8], (*cmd)[0] % 0x1000, right, left);
            func_8016936c(song_data[8], (*cmd)[0] % 0x1000);
            func_80168f50(song_data[8], (*cmd)[0] % 0x1000, 1, 1);
            break;
        /* The empty cases stay: with a default instead they differ in 52 instruction lines. */
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 10:
            break;
        case 9:
            if (((*cmd)[0] % 0x1000) == 0xfff) {
                func_80165d34(((u8 *)cmd)[2], ((u8 *)cmd)[2]);
            } else {
                func_80164a78((*cmd)[0] % 0x1000, ((u8 *)cmd)[2], ((u8 *)cmd)[2]);
            }
            break;
        case 11:
            chan = (*cmd)[0] % 0x1000;
            if (chan == 0xfff) {
                func_80164bbc(0);
            } else {
                if (alt != 0) {
                    chan = ((*cmd)[0] % 0x1000) + 4;
                }
                chan_idx = chan;
                (base + chan_idx)[0x748c] = 0;
                func_8016452c(chan_idx);
            }
            break;
        }
    /* Not changed from the original: the loop test reads the second word after the command has run. */
    } while (((*cmd++)[1] & 0x800000));
}
