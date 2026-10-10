/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * original also sets a saved register to zero that nothing reads (12 bytes
 * with its save and restore), and the stack frame differs (inferred, not
 * tested; the built code is 148 bytes against the original's 232, which
 * these two items do not account for). The exact owner
 * of the bytes in the PS1 build stays the raw bytes of the module image; the
 * build does not use this file. The differential test next to it
 * (difftest.py) compares the behavior of this C with the original code on
 * random inputs of the contract below.
 *
 * What it does (inferred, not an original name): lists the files of one
 * memory card. The device name is chosen by the card number (card 0 takes
 * the first 8-byte name of data_800df11c_slot0f, any other card the
 * second). firstfile is asked for the first directory entry; when it finds
 * one, the entry's name is copied with strcpy into a table of 0x28-byte
 * slots at data_800f80a4_slot0f + card * 0x258, and nextfile is asked for
 * the next entry until it returns something other than the address of the
 * entry buffer (the models return 0 when they find none).
 *
 * Contract (the roles named are inferred):
 *   Argument: a0 = card (the setup uses 0 to 3, over a table of 4 * 0x258
 *     bytes; a larger value would write outside it and is not tested; the
 *     slots written per call are bounded by the setup's 1 to 15 entries,
 *     15 slots of 0x28 bytes being 0x258). No result.
 *   Reads: data_800df11c_slot0f (the two names). Writes: through strcpy,
 *     the slots at data_800f80a4_slot0f + card * 0x258 + k * 0x28.
 *   Callees are Sony's library, all MODELS written by the contract (the
 *   names, the directory entry and the string copy are the library's as
 *   known from its documentation and from how this function uses the
 *   results; they were not run):
 *     firstfile(name, entry): the model copies a 10-word directory entry
 *       of the setup's making into the buffer `entry` and returns its
 *       address, or returns 0 (not found), as the setup chose;
 *     nextfile(entry): same, from a list of 0 to 14 entries of the setup's
 *       making (1 to 15 slots in all with the first), found for each in
 *       turn and then 0;
 *     strcpy(dest, src): copies the bytes of src up to and including the
 *       terminating 0, returns dest.
 *   The argument values that are addresses of the function's own locals
 *   (the entry buffer) are not logged, since the two codes place their
 *   frames differently; the name's two words and the string copied (10
 *   words from the entry) are logged as pointees. The first argument of
 *   strcpy (a table slot) is logged.
 *   Watched: nothing. The function writes memory only through strcpy,
 *     which is a model that writes it, so no callee reads state the function
 *     wrote.
 *   Not reached by any input: none expected; the test reports the slots.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations; the headers lack them. */
extern Slot0fRec11c data_800df11c_slot0f;
extern Slot0fRec80a4 data_800f80a4_slot0f[];
Slot0fRec80a4 *firstfile(u8 *name, Slot0fRec80a4 *entry);
Slot0fRec80a4 *nextfile(Slot0fRec80a4 *entry);
char *strcpy(char *dest, const char *src);

void func_800e0f2c_slot0f(int card) {
    Slot0fRec80a4 entry;
    u8 *name;
    int offset;

    name = data_800df11c_slot0f.first;
    if (card != 0) {
        name = data_800df11c_slot0f.second;
    }
    if (firstfile(name, &entry) == &entry) {
        offset = 0;
        do {
            strcpy((char *)data_800f80a4_slot0f + card * 0x258 + offset, (char *)&entry);
            offset += 0x28;
        } while (nextfile(&entry) == &entry);
    }
}
