/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_80155f98(int a, int b) {
    u32 carry = 0;
    u32 res = 0;
    u32 mask = 0xf;
    u32 ten = 10;
    u32 next = 0x10;
    u16 i;
    for (i = 0; i < 8; i++) {
        u32 v = a & mask;
        v += (b & mask) + carry;
        carry = 0;
        if (v >= ten) {
            carry = next;
            v -= ten;
        }
        res |= v;
        mask <<= 4;
        ten <<= 4;
        next <<= 4;
    }
    if (res > 0x9999998) {
        res = 0x9999999;
    }
    return res;
}

int func_80156018(int a, int b) {
    int borrow = 0;
    int res = 0;
    int mask = 0xf;
    int ten = 10;
    int next = 0x10;
    u16 i;
    for (i = 0; i < 7; i++) {
        int d = a & mask;
        int sub = (b & mask) + borrow;
        d = d - sub;
        if (d < 0) {
            d = (a & mask) + ten;
            d = d - sub;
            borrow = next;
        } else {
            borrow = 0;
        }
        res |= d;
        mask <<= 4;
        ten <<= 4;
        next <<= 4;
    }
    return res;
}

void func_80156084(void) {
    data_8018d268 = 0;
}

void func_80156094(void) {
    table_80181650[data_8018d268]();
}
