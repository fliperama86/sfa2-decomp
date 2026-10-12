/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/*
 * Walks backward over words of memory, 29 words at most, starting at the low
 * 24 bits of the argument (inferred, not an original name). A word that holds
 * the address of the word before it is a link; for each run of links the
 * first word of the run (the highest one) is set to the address of the first
 * word below the run that is not a link, so that the run is skipped at once.
 * Words outside a run are left alone.
 */
void compact_linked_cell_runs(void *start) {
    int steps_left = 29;
    u32 *cell;
    u32 *run_top;
    u32 link;

    cell = (u32 *)((u32)start & 0xffffff);
    for (;;) {
        /* The loop tests first and loops back from inside the branch. Written
           as while ((link = *cell) != ...) it differs in 16 of the original's
           33 instruction lines, the second loop below in 17, both in 32. */
        for (;;) {
            link = *cell;
            if (link != (u32)(cell - 1)) {
                if (--steps_left == 0)
                    return;
                cell--;
                continue;
            }
            break;
        }
        if (--steps_left == 0)
            return;
        run_top = cell;
        cell--;
        for (;;) {
            link = *cell;
            if (link == (u32)(cell - 1)) {
                if (--steps_left == 0)
                    return;
                cell--;
                continue;
            }
            break;
        }
        /* The store to the top of the run is made even when the count runs
           out right after it. */
        *run_top = (u32)cell;
        if (--steps_left == 0)
            return;
        cell--;
    }
}
