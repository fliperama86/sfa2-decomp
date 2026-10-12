/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Constant strings of the module's data (6 bytes with their end, 3 for the third), declared as words and halfwords so that the compiler knows their alignment; the copies below stay inside them. */
extern u32 data_800df0f0_slot0f[2];
extern u32 data_800df0f8_slot0f[2];
extern char data_800df100_slot0f[];
extern Slot0fRec8504 *data_800e8504_slot0f;
char *strcat(char *dest, const char *src);
int close(int fd);
int func_800e0da8_slot0f(int mode);
int func_800e11e4_slot0f(const char *name, int flags);
long func_800e1250_slot0f(long fd, void *buf, long n);
void func_800e1348_slot0f(void);

/*
 * What it does (inferred, not an original name): a save-file load from the
 * memory card. It builds a file path at data_8018fef8 + 0x1c: a short device
 * prefix (one of two, chosen by the mode, 0 or not) followed by the text
 * data_800df100_slot0f. func_800e0da8_slot0f is asked about the card (mode as
 * argument); its answers 1 and 4 are returned as they are, 3 becomes 2, any
 * other continues. The file is opened (func_800e11e4_slot0f, flag 1), 0x2000
 * bytes are read into the block that data_800e8504_slot0f points at, and the
 * file is closed. Failures return 3 (open: result -1) or 2 (read: result
 * below 0; close: result -1); success calls func_800e1348_slot0f and
 * returns 0.
 */
int func_800e0a1c_slot0f(int mode) {
    Slot0fRec8504 *hdr = data_800e8504_slot0f;
    /* The buffer's address in a local: the original loads it into a register at entry and forms the path from that register. With data_8018fef8 written at each use, this function differs from the original in 10 instruction slots. */
    int *base = data_8018fef8;
    char *path;
    int result;
    int fd;

    /* The compiler's built-in copy: this compiler expands it in line as one block move, which is what the original's instructions show (a word and a halfword loaded, then stored). The original most likely copied a string literal with strcpy (inferred); copying from the module's data keeps this unit free of data of its own. Written as two separate stores, this function differs from the original in 31 instruction slots. */
    if (mode == 0) {
        __builtin_memcpy(&base[7], data_800df0f0_slot0f, 6);
    } else {
        __builtin_memcpy(&base[7], data_800df0f8_slot0f, 6);
    }
    path = (char *)(base + 7);
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
    fd = func_800e11e4_slot0f(path, 1);
    if (fd == -1) {
        return 3;
    }
    if (func_800e1250_slot0f(fd, hdr, 0x2000) < 0) {
        return 2;
    }
    if (close(fd) == -1) {
        return 2;
    }
    func_800e1348_slot0f();
    return 0;
}
