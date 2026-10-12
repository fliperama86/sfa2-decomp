/*
 * The port's C for func_80120604. It runs in the PC program in place of the
 * C of the unit resident/sound/func_80120604.c; the PS1 build does not use this file, and the
 * unit keeps the exact C.
 *
 * What is different from the unit's C: the shift count is masked with 31 in
 * the argument of the call to func_8016a7e4 in command 0. The unit shifts
 * by the count as it is.
 *
 * Why: the count is the channel, bits 7 to 11 of the entry's second word
 * (0 to 31), plus 4 when alt is not 0, so it can reach 35. The original
 * shifts with a variable shift instruction (sllv at 80120708), and the console takes
 * the count modulo 32 (read from the original's listing, not tested here).
 * In C a shift by 32 or more has no defined result, so compiled for a PC
 * nothing ties the unit's C to the console's result for such a count. With
 * the mask written the count is defined, and for counts below 32 this file
 * and the unit do the same. The readable C that stood for this function
 * before it was exact (ps1/src/resident_nonmatching/func_80120604.c) wrote
 * no mask; its test built it with the console's compiler, where the
 * instruction gives the modulo. Whether the game's data gives a channel of
 * 28 or more with alt not 0 is not known.
 *
 * What it does (inferred): see the comment above the function.
 *
 * Contract:
 *   Arguments: song (0 to 0x7f, or 0x80), alt, start (16 bits), pos (only
 *     its low 16 bits are used). No return value.
 *   Reads: table_8016e6e0[song]; the entries; data_80190a44[song] and
 *     [song + 8]; the byte at data_80190a44 + 0x748c + channel.
 *   Writes: the byte at data_80190a44 + 0x748c + channel (data_80197ed0;
 *     command 11 reaches a channel of up to 0xffe + 4), the halfword at
 *     data_80190a44 + 0x74b4 + 2 * song for song below 2 (table_80197ef8).
 *     The setup fills the 0x1004 bytes from data_80197ed0 with random
 *     bytes, so that a far store is compared at the end of the case.
 *   Watched at every call (copied into the log): the 36 bytes of
 *     data_80197ed0 and the word at table_80197ef8.
 *   Callees replaced by recorders, the same in both runs (all in Sony's
 *     library, from 0x80164000; inferred from their addresses):
 *     func_80164bbc (1 argument), func_8016a7e4 (1 argument; returns 2 in
 *     half of the cases and 0 otherwise, the same for all its calls in a
 *     case), func_80164140 (8 arguments), func_80164ef0 (4), func_8016936c
 *     (2), func_80168f50 (4), func_80165d34 (2), func_80164a78 (3),
 *     func_8016452c (1). All other results are 0.
 *   Excluded inputs: song above 0x80 (the original indexes past the table;
 *     inferred).
 *   Not changed from the original: the loop test reads the second word
 *     after the command has run.
 *   Not reached by any input: none expected; see the coverage line.
 */
#include "../../ps1/src/game.h"
#include "../../ps1/src/protos.h"
#include "../../ps1/src/externs.h"

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
                /* The override: the count is masked, as the console's shift does it. */
                if (func_8016a7e4(1 << (chan_idx & 31)) == 2) {
                    break;
                }
            }
            /* The field is read again; storing prio differs in 98 instruction lines. */
            (base + chan_idx)[0x748c] = ((*cmd)[1] >> 16) & 0x7f;
            /* The local holds a constant that no instruction reads: the original sets a register to this value here and never uses it. It is a stand-in for a variable of the original source, not recovered source. The name is inferred. */
            /* The original's instruction: move s0,zero at 80120728; nothing reads s0 after it. With 0 written at the local's use this function differs from the original in 25 instruction lines. */
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
               The definition (resident/sound/func_80164ef0.c) takes its last two parameters as unsigned short.
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
