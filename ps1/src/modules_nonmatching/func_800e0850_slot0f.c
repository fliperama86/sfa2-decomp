/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register allocation and
 * instruction order. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the module image; the build does not use this file.
 * The differential test next to it (func_800e0850_slot0f.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): writes a memory-card style
 * file. It builds a path in the buffer at data_8018ff14 (a word and a
 * halfword chosen by mode, then a string appended), asks func_800e0da8
 * about the card (results 1 and 4 are returned as they are, 3 becomes 2),
 * opens the file for reading and writing; when that fails it opens it again
 * with the create flag (failure returns 3), closes that handle (failure
 * returns 2) and opens it once more. It then fills the header block that
 * data_800e8504_slot0f points at (a halfword, the bytes 0x11 and 1 at
 * offsets 2 and 3, the name copied in at offset 4, 28 bytes from offset
 * 0x44 cleared), calls func_800e13dc, and writes 0x2000 bytes of the block
 * with func_800e12cc (failure returns 2). Otherwise it closes the file and
 * returns 1 when the close failed, 0 when it did not.
 *
 * Contract (what the code reads and writes; the roles named are inferred):
 *   Arguments: a0 = mode (zero or not), a1 = pointer to a name string.
 *   Result: v0, an int; it is compared.
 *   Reads: data_800e8504_slot0f (a pointer), data_800df0f0 to data_800df0fc
 *     (the word and halfword for each mode), data_800df100 (the string
 *     appended), data_800df118 (halfword), the name string.
 *   Writes: data_8018ff14 (word) and data_8018ff18 (halfword); the header
 *     block: halfword at 0, bytes at 2 and 3, the 28 bytes from 0x44.
 *     The original also stores the byte at data_800df11a into offset 2 of
 *     the block, and overwrites it at once with 0x11; this C omits that
 *     store, as no callee runs between and nothing observes it.
 *   Callees replaced by recorders (they reach the library or the card):
 *     strcat (2 args; the log copies the destination 2 words and the
 *     source 4 words), func_800e0da8 (1 arg, results chosen per case),
 *     func_800e11e4 (2 args, results in turn), close (1 arg, results in
 *     turn), strcpy (2 args; source 4 words), func_800e13dc (no
 *     arguments), func_800e12cc (3 args, the result chosen per case).
 *     strcat and strcpy therefore do not change their destinations in the
 *     test; the destination is part of the watch below.
 *     Watched at every call: the header block (0x60 bytes) and
 *     data_8018ff14 with three words after it.
 *   Aliasing: the header block, the name string and the path buffer are
 *     distinct.
 *   Inputs excluded: none. The name string must be 16 readable bytes (the
 *     recorder copies 4 words).
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_800df0f0_slot0f;
extern s16 data_800df0f4_slot0f;
extern u32 data_800df0f8_slot0f;
extern s16 data_800df0fc_slot0f;
extern char data_800df100_slot0f[];
extern s16 data_800df118_slot0f;
extern Slot0fRec8504 *data_800e8504_slot0f;
extern u32 data_8018ff14;
extern s16 data_8018ff18;
char *strcat(char *dest, const char *src);
char *strcpy(char *dest, const char *src);
int close(int fd);
int func_800e0da8_slot0f(int mode);
int func_800e11e4_slot0f(char *path, int flags);
int func_800e12cc_slot0f(int fd, void *buf, int len);
void func_800e13dc_slot0f(void);

int func_800e0850_slot0f(int mode, char *name) {
    Slot0fRec8504 *hdr = data_800e8504_slot0f;
    char *path = (char *)&data_8018ff14;
    int result;
    int fd;
    int n;
    u8 *z;

    if (mode == 0) {
        data_8018ff14 = data_800df0f0_slot0f;
        data_8018ff18 = data_800df0f4_slot0f;
    } else {
        data_8018ff14 = data_800df0f8_slot0f;
        data_8018ff18 = data_800df0fc_slot0f;
    }
    strcat(path, data_800df100_slot0f);
    result = func_800e0da8_slot0f(mode);
    if (result == 1) {
        return 1;
    }
    if (result == 4) {
        return 4;
    }
    if (result == 3) {
        return 2;
    }
    fd = func_800e11e4_slot0f(path, 2);
    if (fd == -1) {
        fd = func_800e11e4_slot0f(path, 0x10200);
        if (fd == -1) {
            return 3;
        }
        if (close(fd) == -1) {
            return 2;
        }
        fd = func_800e11e4_slot0f(path, 2);
    }
    hdr->field_00 = data_800df118_slot0f;
    hdr->field_02 = 0x11;
    hdr->field_03 = 1;
    z = &hdr->field_44;
    for (n = 0x1c; n != 0; n--) {
        *z++ = 0;
    }
    strcpy((char *)hdr + 4, name);
    func_800e13dc_slot0f();
    if (func_800e12cc_slot0f(fd, hdr, 0x2000) != -1) {
        return close(fd) == -1;
    }
    return 2;
}
