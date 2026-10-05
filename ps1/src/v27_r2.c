/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: NOT EXACT (2 residuals, same in both functions): original keeps the first loop bound
   copied (move a0,v0) after the blez, and puts the flag pointer in t5 and the block
   counter in t4; mine has them swapped. Everything else matches. */
/* Form found by automatic permutation search, then cleaned by hand. */
void func_8011e790(unsigned int n, unsigned char *src, unsigned int dst, unsigned int key) {
    unsigned short flags[4];
    unsigned char value;
    unsigned short i, j, count;
    int blocks;
    unsigned short *fp;
    int left, old;
    unsigned short cnt;
    unsigned int bits;

    n &= 0xff;
    for (i = 0; (int) i < ((int) (n - 1) >> 4) + 1; i++) {
        flags[i] = *(unsigned short *) src;
        src += 2;
    }
    fp = flags;
    blocks = n >> 4;
    n &= 0xf;
    left = blocks;
    left = left - 1;
    if (blocks != 0) {
        do {
            bits = *fp++;
            for (i = 0; i < 0x10; i++, bits >>= 1) {
                if (bits & 1) {
                    value = *src++;
                    count = *src++;
                    for (j = 0; j < count; j++)
                        *(unsigned char *) (dst++ ^ key) = value;
                } else {
                    *(unsigned char *) (dst++ ^ key) = *src++;
                }
            }
            old = left;
            left--;
        } while ((unsigned short) old != 0);
    }
    bits = *fp;
    cnt = n;
    for (i = 0; i < cnt; i++, bits >>= 1) {
        if (bits & 1) {
            value = *src++;
            count = *src++;
            for (j = 0; j < count; j++)
                *(unsigned char *) (dst++ ^ key) = value;
        } else {
            *(unsigned char *) (dst++ ^ key) = *src++;
        }
    }
}
