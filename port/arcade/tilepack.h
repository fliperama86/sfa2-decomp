/* The PS1's sprite tile chunk (table 0 slot 1 or 2 of a fighter file) built from the tiles, and a reader for it.
 *
 * The format is the one `decode_compressed_block` reads (tilepack.py has the description and the measurements): a table of one
 * 32-bit little-endian offset a tile, then one block a tile, adjacent, no padding. A block is 64 literal words, or LZ (items with
 * flag bits, a back reference is 2 to 5 words from at most 16,383 words before the block), or a 64-bit mask of non-zero words
 * and those words. This encoder is not the PS1's own (whose choices are not reproducible): it makes the smallest chunk it
 * can find, which decodes to the same tiles, and fails when that does not fit the room the PS1 layout has.
 *
 * Project-authored, plain C99. No byte of either game is here. */
#ifndef ARCADE_TILEPACK_H
#define ARCADE_TILEPACK_H

#include <stddef.h>
#include <stdint.h>

/* Build the chunk of `count` tiles (the PS1's 4-bit tiles, 128 bytes each, index 0 first: 16 rows of 8 bytes, the left pixel of
 * a pair in the low nibble) into out[0..room). On success returns 0 and *size is the chunk's size. When the chunk would be larger
 * than `room` (or on a bad argument, or out of memory) returns -1 with a line in err, writes nothing to out and does not touch
 * a byte of it beyond room. err may be NULL. */
int arcade_tilepack(const uint8_t (*tiles)[128], uint32_t count, uint8_t *out, uint32_t room, uint32_t *size, char *err, size_t errsize);

/* The reader, for tests and checks: the number of tiles of a chunk (0 when the table is not plausible), and tile `index` into
 * tile[128] (all four block forms, every offset and length checked against `size`). Returns 0, or -1 with a line in err. */
uint32_t arcade_tilepack_count(const uint8_t *chunk, uint32_t size);
int arcade_tilepack_decode(const uint8_t *chunk, uint32_t size, uint32_t index, uint8_t tile[128], char *err, size_t errsize);

#endif
