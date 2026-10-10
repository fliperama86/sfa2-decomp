/*
 * Nonmatching. This function is claimed to be a behavioral match only: the
 * code built from this C has the original's size (132 bytes) but its bytes
 * have not been compared with the original's and are not claimed to be
 * identical. The exact owner of the bytes in the PS1 build stays the raw
 * bytes of the resident image; the build does not use this file. The
 * differential test next to it (difftest.py) compares the behavior of this
 * C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): walks backward over words
 * of memory, 29 words at most, starting at the low 24 bits of the argument.
 * A word that holds the address of the word before it (a link to the
 * previous cell) belongs to a run of such links. For each run the walk
 * stores, into the first word of the run (the highest one), the address of
 * the first word below the run that is not a link, so that the run is
 * skipped at once. Words that are not part of a run are left alone.
 *
 * Contract:
 *   Argument: a0 = an address; only its low 24 bits are used (the access
 *     goes to that address as a plain integer, which on the console is the
 *     RAM mirror at the bottom of the address space and in the test is the
 *     same physical RAM).
 *   No return value.
 *   Reads: up to 29 words, descending from the start address in steps of 4.
 *   Writes: at most the first word of each run found (a word that was a
 *     link), with the address of the first word below that run that is not
 *     a link (a word that does not hold the address of the one before it).
 *     The store is made even when the step counter runs out at that point.
 *   The words read stay inside RAM: the start address is at least 29 words
 *     above the start of the RAM mirror and the memory below it is readable.
 *   Nothing is called.
 *   Every instruction slot of the original is reachable by an input (the
 *     counter reaching 0 at each of its five decrements).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80119694(void *a) {
    u32 *p;
    u32 *last;
    int n;

    n = 29;
    p = (u32 *)((u32)a & 0xffffff);
    for (;;) {
        if (*p != (u32)(p - 1)) {
            if (--n == 0) {
                return;
            }
            p--;
            continue;
        }
        if (--n == 0) {
            return;
        }
        last = p;
        p--;
        while (*p == (u32)(p - 1)) {
            if (--n == 0) {
                return;
            }
            p--;
        }
        *last = (u32)p;
        if (--n == 0) {
            return;
        }
        p--;
    }
}
