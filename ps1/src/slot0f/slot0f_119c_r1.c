/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int open(const char *devname, int flags);
long read(long fd, void *buf, long n);

void func_800e119c_slot0f(int *p) {
    func_8015785c(p[4]);
    func_8015785c(p[5]);
    func_8015785c(p[6]);
}

void func_800e11e4_slot0f(const char *name, int flags) {
    int i = 0x78;
    do {
        func_8015fb30(0);
        if (open(name, flags) != -1) {
            break;
        }
    } while (--i != 0);
}

void func_800e1250_slot0f(long fd, void *buf, long n) {
    int i = 0x78;
    do {
        func_8015fb30(0);
        if (read(fd, buf, n) != -1) {
            break;
        }
    } while (--i != 0);
}
