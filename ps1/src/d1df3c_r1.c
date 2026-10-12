/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

void func_8011e5c8(u16 n, u16 *src, u32 dst, u32 key);
void decode_rle(u32 n, u16 *src, u32 dst, u32 key);

/*
 * What it does (inferred): unpacks one compressed texture block into
 * 16-bit cells. Bits 0 to 13 of the flags index a table of offsets at the
 * start of the data, bit 15 selects whether each cell has its four nibbles
 * reversed, bits 14 and 15 pick one of four address masks. Each cell is
 * stored at the running destination exclusive-or the mask. The block starts
 * with a halfword n: 0 means 64 literal cells; a high byte of 0 means a
 * count of items, each a literal or a (length, distance) reference back
 * into the block, chosen by one flag bit; a high byte of 0x80 means 64 flag
 * bits choosing a literal or a zero cell; any other value is handed to
 * func_8011e5c8 (bit 15 set) or decode_rle (clear), which take the
 * second mask (key).
 */
void decode_compressed_block(void *data, int b, u8 *out) {
    unsigned flags = b;
    /* Room for 4 flag halfwords, as the original frame has; a count above 64 items would write past it (flagged, inferred). */
    u16 flag_words[4];
    /* The original copies these two tables from initialized data to the stack. */
    u16 mask_table[4] = {0x0000, 0x0078, 0x0006, 0x007e};
    u16 key_table[4] = {0x0000, 0x0078, 0x0007, 0x007f};
    u16 *p, *src, *start, *flag_p;
    u16 bits, word, len, cell, groups, groups_left, n;
    unsigned i, j;

    u32 mask = mask_table[(flags >> 14) & 3];
    u32 key = key_table[(flags >> 14) & 3];
    if (flags & 0x8000) {
        p = (u16 *)(data + ((int *)data)[flags & 0x3fff]);
        start = p;
        n = *p++;
        if (n == 0) {
            for (i = 0; i < 64; i++) {
                word = *p++;
                *(u16 *)((u32)out ^ mask) = (word >> 12) | ((word & 0xf00) >> 4) | ((word & 0xf0) << 4) | ((word & 0xf) << 12);
                out += 2;
            }
        } else if ((n & 0xff00) == 0) {
            n &= 0xff;
            for (i = 0; i < ((n - 1) >> 4) + 1; i++) flag_words[i] = *p++;
            flag_p = flag_words;
            /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
            do {
                groups = n >> 4;
                groups_left = groups - 1;
                n &= 0xf;
            } while (0);
            if (groups != 0) {
                do {
                    bits = *flag_p++;
                    for (i = 0; i < 16; i++) {
                        if (bits & 1) {
                            word = *p++;
                            len = (word & 3) + 2;
                            src = start - (word >> 2);
                            for (j = 0; j < len; j++) {
                                word = *src++;
                                *(u16 *)((u32)out ^ mask) = (word >> 12) | ((word & 0xf00) >> 4) | ((word & 0xf0) << 4) | ((word & 0xf) << 12);
                                out += 2;
                            }
                        } else {
                            word = *p++;
                            *(u16 *)((u32)out ^ mask) = (word >> 12) | ((word & 0xf00) >> 4) | ((word & 0xf0) << 4) | ((word & 0xf) << 12);
                            out += 2;
                        }
                        bits >>= 1;
                    }
                } while ((groups = groups_left--) != 0);
            }
            bits = *flag_p;
            for (i = 0; i < n; i++) {
                if (bits & 1) {
                    word = *p++;
                    len = (word & 3) + 2;
                    src = start - (word >> 2);
                    for (j = 0; j < len; j++) {
                        word = *src++;
                        *(u16 *)((u32)out ^ mask) = (word >> 12) | ((word & 0xf00) >> 4) | ((word & 0xf0) << 4) | ((word & 0xf) << 12);
                        out += 2;
                    }
                } else {
                    word = *p++;
                    *(u16 *)((u32)out ^ mask) = (word >> 12) | ((word & 0xf00) >> 4) | ((word & 0xf0) << 4) | ((word & 0xf) << 12);
                    out += 2;
                }
                bits >>= 1;
            }
        } else if ((n & 0xff00) == 0x8000) {
            for (i = 0; i < 4; i++) flag_words[i] = *p++;
            flag_p = flag_words;
            groups_left = 3;
            do {
                bits = *flag_p++;
                for (i = 0; i < 16; i++) {
                    if (bits & 1) {
                        word = *p++;
                        *(u16 *)((u32)out ^ mask) = (word >> 12) | ((word & 0xf00) >> 4) | ((word & 0xf0) << 4) | ((word & 0xf) << 12);
                        out += 2;
                    } else {
                        *(u16 *)((u32)out ^ mask) = 0;
                        out += 2;
                    }
                    bits >>= 1;
                }
            } while ((groups = groups_left--) != 0);
        } else {
            /* The callee gets the second mask (key); it reads only the low byte of n (inferred). */
            func_8011e5c8(n, p, (u32)out, key);
        }
    } else {
        p = (u16 *)(data + ((int *)data)[flags & 0x3fff]);
        start = p;
        n = *p++;
        if (n == 0) {
            for (i = 0; i < 64; i++) {
                cell = *p++;
                *(u16 *)((u32)out ^ mask) = cell;
                out += 2;
            }
        } else if ((n & 0xff00) == 0) {
            n &= 0xff;
            for (i = 0; i < ((n - 1) >> 4) + 1; i++) flag_words[i] = *p++;
            flag_p = flag_words;
            groups = n >> 4;
            groups_left = groups - 1;
            n &= 0xf;
            if (groups != 0) {
                do {
                    bits = *flag_p++;
                    for (i = 0; i < 16; i++) {
                        if (bits & 1) {
                            word = *p++;
                            len = (word & 3) + 2;
                            src = start - (word >> 2);
                            for (j = 0; j < len; j++) {
                                cell = *src++;
                                *(u16 *)((u32)out ^ mask) = cell;
                                out += 2;
                            }
                        } else {
                            cell = *p++;
                            *(u16 *)((u32)out ^ mask) = cell;
                            out += 2;
                        }
                        bits >>= 1;
                    }
                } while ((groups = groups_left--) != 0);
            }
            bits = *flag_p;
            for (i = 0; i < n; i++) {
                if (bits & 1) {
                    word = *p++;
                    len = (word & 3) + 2;
                    src = start - (word >> 2);
                    for (j = 0; j < len; j++) {
                        cell = *src++;
                        *(u16 *)((u32)out ^ mask) = cell;
                        out += 2;
                    }
                } else {
                    cell = *p++;
                    *(u16 *)((u32)out ^ mask) = cell;
                    out += 2;
                }
                bits >>= 1;
            }
        } else if ((n & 0xff00) == 0x8000) {
            for (i = 0; i < 4; i++) flag_words[i] = *p++;
            flag_p = flag_words;
            groups_left = 3;
            do {
                bits = *flag_p++;
                for (i = 0; i < 16; i++) {
                    if (bits & 1) {
                        cell = *p++;
                        *(u16 *)((u32)out ^ mask) = cell;
                        out += 2;
                    } else {
                        *(u16 *)((u32)out ^ mask) = 0;
                        out += 2;
                    }
                    bits >>= 1;
                }
            } while ((groups = groups_left--) != 0);
        } else {
            decode_rle(n, p, (u32)out, key);
        }
    }
}
