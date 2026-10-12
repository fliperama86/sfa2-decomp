/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

int open(const char *devname, int flags);
long read(long fd, void *buf, long n);

void test_events_fifth_to_seventh_of_array_slot0f(int *p) {
    func_8015785c(p[4]);
    func_8015785c(p[5]);
    func_8015785c(p[6]);
}

/* Returns the last result of the call it repeats: both callers use it. */
int func_800e11e4_slot0f(const char *name, int flags) {
    int i = 0x78;
    int fd;
    do {
        func_8015fb30(0);
        fd = open(name, flags);
        if (fd != -1) {
            break;
        }
    } while (--i != 0);
    return fd;
}

/* Returns the last result of the call it repeats: both callers use it. */
long func_800e1250_slot0f(long fd, void *buf, long n) {
    int i = 0x78;
    long r;
    do {
        func_8015fb30(0);
        r = read(fd, buf, n);
        if (r != -1) {
            break;
        }
    } while (--i != 0);
    return r;
}
