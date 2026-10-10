/* The decoder of the disc's streamed audio (CD-ROM XA ADPCM). Pure: no system header but stdint.h and string.h, no global; a
 * native test (port/tools/test_hostxa.py) compiles xa.c alone.
 *
 * One raw sector (2,352 bytes) in; stereo frames of 16 bits at the SOURCE rate out (a mono sector is put on both sides). A second
 * function turns frames of the source rate into frames of 44,100 Hz by LINEAR interpolation. THE CONSOLE DOES NOT RESAMPLE THIS
 * WAY (its own filter has more points); linear interpolation is this project's choice, and its output differs from the console's
 * in the high frequencies.
 *
 * The sector, by the console's hardware documentation (the description of CD-ROM XA ADPCM in the "psx-spx" document): the
 * subheader at byte 16 is file, channel, submode, coding information, repeated at byte 20 (a sector whose two copies differ is
 * refused). The sound data are 18 groups of 128 bytes from byte 24. A group: 16 bytes of headers (the header of block b is at
 * byte 4 + b; bytes 0 to 3 and 12 to 15 are copies), then 112 bytes: 28 words of 4 bytes. A block is 28 samples. With 4 bits
 * there are 8 blocks to a group, and block b takes the nibble (b & 1) of byte (b >> 1) of every word. A header byte: bits 0 to 3
 * the range, bits 4 and 5 the filter. A sample is
 *     s = (t << shift) + ((old * f0 + older * f1 + 32) >> 6)
 * with t the signed nibble, shift = 12 - range, (f0, f1) = (0, 0), (60, 0), (115, -52), (98, -55) by the filter, then clamped to
 * 16 bits; the clamped value is the history (old, older). Two values of history per channel are carried from sector to sector.
 * In stereo the even blocks are the left channel and the odd ones the right; in mono the blocks follow each other. Coding byte: bit
 * 0 stereo, bit 2 18,900 Hz (else 37,800), bit 4 8 bits (refused here).
 *
 * Two readings of this project, neither verified on hardware: the shift ">> 6" of a negative value rounds down (an arithmetic
 * shift, not a division that truncates toward zero); and a range above 12 is taken as 9 (the sound chip's documented behaviour;
 * nothing here shows that the console's XA does the same). */
#ifndef PORT_XA_H
#define PORT_XA_H

#include <stdint.h>

#define XA_SECTOR 2352u
#define XA_MAX_FRAMES 4032u   /* the most one sector decodes to (mono, 18 groups of 8 blocks of 28) */
#define XA_OUT_RATE 44100u

enum { XA_OK = 0, XA_BAD_SUBHEADER = -1, XA_EIGHT_BIT = -2, XA_NOT_AUDIO = -3 };

struct xa_info {
    uint8_t file, channel, submode, coding;
    int stereo, rate, bits;     /* rate in Hz, 37800 or 18900 */
    unsigned frames;            /* frames the sector decodes to at the source rate */
};

struct xa_state {
    int hist[2][2];             /* [channel][0 = last, 1 = the one before] */
    /* the resampler */
    int have_prev;
    int16_t prev[2];            /* the last source frame of the previous call */
    uint32_t phase;             /* position of the next output, in 1/44100 of a source frame after `prev` */
};

void xa_reset(struct xa_state *s);
/* Read the subheader. XA_OK, or a refusal: XA_BAD_SUBHEADER (the copies differ), XA_NOT_AUDIO (the submode's audio bit 0x04 is
 * clear), XA_EIGHT_BIT. The state is not touched. */
int xa_parse(const uint8_t *raw, struct xa_info *info);
/* Decode: out holds info->frames stereo frames (L, R, L, R ...), room for XA_MAX_FRAMES frames. The history is updated only on
 * XA_OK. */
int xa_decode(struct xa_state *s, const uint8_t *raw, int16_t *out, struct xa_info *info);
/* Frames at the output rate that `n` frames of a source of `rate` Hz make at most. */
unsigned xa_resampled_bound(unsigned n, unsigned rate);
/* Resample n source frames to 44,100 Hz into dst (room for xa_resampled_bound(n, rate) frames); returns the count. The first
 * output of a stream is the first source frame; the stream is taken as that frame once more before it, and an output exists once
 * the frame after its position is known. */
unsigned xa_resample(struct xa_state *s, const int16_t *src, unsigned n, unsigned rate, int16_t *dst);

#endif
