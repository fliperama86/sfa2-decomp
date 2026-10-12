/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Constant strings of the module's data (6 bytes with their end, 3 for the third), declared as words and halfwords so that the compiler knows their alignment; the copies below stay inside them. */
extern u32 data_800df0f0_slot0f[2];
extern u32 data_800df0f8_slot0f[2];
extern s16 data_800df118_slot0f[2];
extern char data_800df100_slot0f[];
extern Slot0fRec8504 *data_800e8504_slot0f;
char *strcat(char *dest, const char *src);
char *strcpy(char *dest, const char *src);
int close(int fd);
int check_memory_card_status_slot0f(int mode);
int func_800e11e4_slot0f(const char *name, int flags);
long func_800e12cc_slot0f(long fd, void *buf, long n);
void copy_game_tables_to_saved_block_slot0f(void);

/*
 * What it does (inferred, not an original name): writes a memory-card style
 * file. It builds a path in the buffer at data_8018fef8 + 0x1c (a six-byte
 * prefix chosen by mode, then a string appended), asks func_800e0da8 about
 * the card (results 1 and 4 are returned as they are, 3 becomes 2), opens
 * the file for reading and writing (flag 2); when that returns -1 it opens
 * it again with the create flag (flag 0x10200; -1 returns 3), closes that
 * handle (-1 returns 2) and opens it once more (flag 2; that result is not
 * checked). It then fills the header block that data_800e8504_slot0f points
 * at (a halfword, the bytes 0x11 and 1 at offsets 2 and 3, 28 bytes from
 * offset 0x44 cleared, then the name copied in at offset 4), calls
 * func_800e13dc, and writes 0x2000 bytes of the block with func_800e12cc.
 * When that write returns -1 the function returns 2. When it returns
 * anything else the function closes the file and returns 1 if the close
 * returned -1, 0 if it did not.
 */
int save_memory_card_file_slot0f(int mode, s16 *name) {
    Slot0fRec8504 *hdr = data_800e8504_slot0f;
    /* Stand-in, not recovered source: the local buf, which is only a copy of hdr, reproduces the original's choice of registers. Without it this function differs from the original in 27 instruction slots. */
    void *buf = hdr;
    /* The buffer's address in a local: the original loads it into a register at entry and forms the path from that register. With data_8018fef8 written at each use, this function differs from the original in 27 instruction slots. */
    int *base = data_8018fef8;
    char *path;
    int result;
    int fd;
    int bad;
    int n;
    u8 *z;

    /* The compiler's built-in copy: this compiler expands it in line as one block move, which is what the original's instructions show (a word and a halfword loaded, then stored). The original most likely copied a string literal with strcpy (inferred); copying from the module's data keeps this unit free of data of its own. Written as two separate stores, this function differs from the original in 22 instruction slots. */
    if (mode == 0) {
        __builtin_memcpy(&base[7], data_800df0f0_slot0f, 6);
    } else {
        __builtin_memcpy(&base[7], data_800df0f8_slot0f, 6);
    }
    path = (char *)(base + 7);
    strcat(path, data_800df100_slot0f);
    result = check_memory_card_status_slot0f(mode);
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
    /* The first open's result is kept in a second local: the original copies it and compares the two later results with the copy, which is -1 in that arm. With -1 written at those two tests, this function differs from the original in 16 instruction slots. */
    bad = fd;
    if (fd == -1) {
        fd = func_800e11e4_slot0f(path, 0x10200);
        if (fd == bad) {
            return 3;
        }
        if (close(fd) == bad) {
            return 2;
        }
        fd = func_800e11e4_slot0f(path, 2);
    }
    /* The three-byte copy: same built-in, see above. Written as the plain halfword store (and nothing for the third byte), this function differs from the original in 24 instruction slots. */
    __builtin_memcpy(hdr, data_800df118_slot0f, 3);
    hdr->header_type = 0x11;
    hdr->header_count = 1;
    z = &hdr->body_start;
    for (n = 0x1c; n != 0; n--) {
        *z++ = 0;
    }
    strcpy((char *)hdr + 4, (char *)name);
    copy_game_tables_to_saved_block_slot0f();
    if (func_800e12cc_slot0f(fd, buf, 0x2000) != -1) {
        return close(fd) == -1;
    }
    return 2;
}
