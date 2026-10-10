/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 276 bytes where the original is 300 (the first command of
 * the folder's page prints both); where the two differ was not itemised.
 * The exact owner of the bytes in the PS1 build stays the raw bytes of the
 * module image; the build does not use this file. The differential test
 * next to it (func_800e0a1c_slot0f.py) compares the behavior of this C with the original code on
 * random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a save-file load from the
 * memory card. It builds a file path at data_8018fef8 + 0x1c: a short device
 * prefix (one of two, chosen by the mode, 0 or not) followed by the text
 * data_800df100_slot0f. func_800e0da8_slot0f is asked about the card (mode as
 * argument); its answers 1 and 4 are returned as they are, 3 becomes 2, any
 * other continues. The file is opened (func_800e11e4_slot0f, flag 1), 0x2000
 * bytes are read into the block that data_800e8504_slot0f points at, and the
 * file is closed. Failures return 3 (open: result -1) or 2 (read: result
 * below 0;
 * close: result -1); success calls
 * func_800e1348_slot0f and returns 0.
 *
 * Contract (the roles named are inferred):
 *   Argument: a0 = mode (any 32-bit value). Result in v0: 0, 1, 2, 3 or 4.
 *   Reads: data_800df0f0_slot0f..data_800df0fc_slot0f (u32, s16, u32, s16),
 *     the text data_800df100_slot0f, data_800e8504_slot0f (a pointer).
 *   Writes: the word at data_8018fef8 + 0x1c and the halfword at + 0x20
 *     (data_8018ff14 and data_8018ff18 are the same addresses).
 *   Callees, all replaced by recorders:
 *     strcat (2 arguments: the path and data_800df100_slot0f, whose first 6
 *       words are logged as the pointee of the second argument), result 0;
 *     func_800e0da8_slot0f (1 argument), result chosen by the setup;
 *     func_800e11e4_slot0f (2 arguments: path, 1), result chosen by the setup;
 *     func_800e1250_slot0f (3 arguments: descriptor, buffer, 0x2000), result
 *       chosen by the setup (the buffer is only passed on, not written);
 *     close (1 argument), result chosen by the setup;
 *     func_800e1348_slot0f (no argument), result 0.
 *   Watched at every call: 16 words from data_8018fef8, which hold the path
 *     the function builds and the callees would read.
 *   Weakness: in the module image data_800df0f4_slot0f and
 *     data_800df0fc_slot0f hold the same value (read from the module image,
 *     not tested here), so
 *     the test cannot tell which of the two the function stores in each
 *     arm.
 *   Not reached by any input: none expected; the test reports the slots.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations; the headers lack them. */
extern u8 data_800df0f0_slot0f[];
extern s16 data_800df0f4_slot0f;
extern u8 data_800df0f8_slot0f[];
extern s16 data_800df0fc_slot0f;
extern char data_800df100_slot0f[];
extern u8 *data_800e8504_slot0f;
char *strcat(char *dest, const char *src);
int close(int fd);
int func_800e0da8_slot0f(int mode);
/* func_800e11e4_slot0f and func_800e1250_slot0f are defined void in their exact units (slot0f_119c_r1.c,
   slot0f_119c_r2.c). The original's code here uses the value the callee
   leaves in the result register, so each call below is made through a cast
   of the callee to a type that returns it; the cast is not an original
   declaration. */
void func_800e11e4_slot0f(const char *name, int flags);
void func_800e1250_slot0f(long fd, void *buf, long n);
void func_800e1348_slot0f(void);

int func_800e0a1c_slot0f(int mode) {
    u8 *buffer = data_800e8504_slot0f;
    char *path = (char *)data_8018fef8 + 0x1c;
    int status;
    int fd;

    if (mode == 0) {
        *(u32 *)path = *(u32 *)data_800df0f0_slot0f;
        *(s16 *)(path + 4) = data_800df0f4_slot0f;
    } else {
        *(u32 *)path = *(u32 *)data_800df0f8_slot0f;
        *(s16 *)(path + 4) = data_800df0fc_slot0f;
    }
    strcat(path, data_800df100_slot0f);
    status = func_800e0da8_slot0f(mode);
    if (status == 1) {
        return 1;
    }
    if (status == 4) {
        return 4;
    }
    if (status == 3) {
        return 2;
    }
    fd = ((int (*)(const char *, int))func_800e11e4_slot0f)(path, 1);
    if (fd == -1) {
        return 3;
    }
    if (((long (*)(long, void *, long))func_800e1250_slot0f)(fd, buffer, 0x2000) < 0) {
        return 2;
    }
    if (close(fd) == -1) {
        return 2;
    }
    func_800e1348_slot0f();
    return 0;
}
