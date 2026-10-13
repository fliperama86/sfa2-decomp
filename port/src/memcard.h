/* The memory card's image and its file system: 128 KB, as the PlayStation's card and every emulator's card file have it. No game, no clock, no
 * files: the routines work on a buffer of MC_SIZE bytes, so a control can run them on made-up images (port/tools/test_hostmemcard.py).
 *
 * Layout (the format of the console's card; the source is the console's documentation, nothing of the game):
 *   frame 0         "MC", 125 zero bytes, a checksum (the XOR of the 127 bytes before it)
 *   frames 1 to 15  one directory entry for each of the 15 blocks (a block is 64 frames, 8,192 bytes): the state (0x51 the first block of a file,
 *                   0x52 a middle one, 0x53 the last of a file of several; 0xa0 free; 0xa1, 0xa2, 0xa3 deleted: the first, a middle, the last),
 *                   the size of the file in bytes (u32, in the first block's entry), the next block of the file (u16: its number less one, 0xffff
 *                   for none), the file's name (20 characters and a zero, in the first block's entry), 96 zero bytes, a checksum of the entry
 *   frames 16 to 35 the list of broken frames (none: 0xffffffff, zeros, 0xffff, zeros, a checksum)
 *   frames 36 to 62 unused (zero)
 *   frame 63        the write test frame: as frame 0
 * The data of block n (1 to 15) is at byte n * 8192. */
#ifndef PORT_MEMCARD_H
#define PORT_MEMCARD_H

#define MC_SIZE   131072u
#define MC_FRAME  128u
#define MC_BLOCK  8192u
#define MC_BLOCKS 15
#define MC_NAME   20u    /* characters of a file name; the entry has room for a zero after them */

/* A blank card: formatted, no file. */
void mc_format(unsigned char *img);
/* 1 when frame 0 reads "MC" and its checksum is right. */
int mc_is_formatted(const unsigned char *img);
/* 1 when `pattern` matches `name`: '?' any one character, '*' anything that is left (the console's firstfile does no more). */
int mc_match(const char *pattern, const char *name);
/* The first block of the file with this name (1 to 15), 0 when there is none. Only the first 20 characters of `name` count. */
int mc_find(const unsigned char *img, const char *name);
/* The next first block of a file, in the order of the blocks, after block `after` (0 for the first) whose name matches `pattern`; 0 when there is none. */
int mc_next(const unsigned char *img, int after, const char *pattern);
/* The name of the file whose first block is `first` (a 21-byte buffer) and its size in bytes. */
void mc_name(const unsigned char *img, int first, char *out21);
unsigned mc_size(const unsigned char *img, int first);
/* The number of free blocks (free and deleted). */
int mc_free_blocks(const unsigned char *img);
/* Creates a file of `blocks` blocks (1 to 15) with the lowest free blocks, size blocks * 8192, its data zero. The first block (1 to 15), or 0 when the name
 * exists, the name is empty or longer than MC_NAME, `blocks` is not 1 to 15, or the free blocks do not suffice. */
int mc_create(unsigned char *img, const char *name, unsigned blocks);
/* Reads or writes up to `n` bytes at byte `offset` of the file starting at `first`, along its blocks, never past its size; the count done. */
unsigned mc_read(const unsigned char *img, int first, unsigned offset, void *buf, unsigned n);
unsigned mc_write(unsigned char *img, int first, unsigned offset, const void *buf, unsigned n);

#endif
