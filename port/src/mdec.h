/* The core of the movie decoder for the PC program: what the console's macroblock decoder chip (the MDEC) does, and the part of
 * the library routine in front of it (the Huffman-style decoding of a frame's bitstream, `DecDCTvlc`). Pure C (mdec.c uses only
 * string.h; no global, no allocation); a native test (port/tools/test_hostmdec.py) compiles mdec.c alone. Nothing calls it yet.
 *
 * WHAT THIS IS NOT. Nothing here is compared with a console's output: the project has no output of a console to compare with.
 * The decoder is checked against hand-worked cases and against references written in the test from the same documents, with
 * bounds where the documents say the arithmetic is not exact (see the test). It is not shown to give the console's pixels.
 *
 * Sources, both chapters of the public "PlayStation Specifications" document (psx-spx):
 *   - chapter "Macroblock Decoder (MDEC)": the chip's commands, the run-level decoding, the inverse transform, the colour
 *     conversion, the zigzag order, the scale table, the 16-bit run-level code (6 bits quantisation scale or run, 10 bits signed
 *     level; FE00h ends a block);
 *   - chapter "CDROM File Formats", its sections on video streams and "BS" compression: the frame's header, the versions, the
 *     table of variable-length codes, the escape code, the end-of-block and end-of-frame codes, the version 3 offsets of DC values.
 *
 * Choices of this project where the documents leave a point open (each is a reading, not the chip's known behaviour):
 *   - THE INVERSE TRANSFORM is the document's "real_idct_core" form (1024 multiplications, uses the scale table that the chip's
 *     third command sets), operation for operation: two passes of sum = src[y+z*8] * (scale[x+z*8] / 8), then (sum + 0xFFF) /
 *     0x2000. The other form of the document (the fast one, with fixed constants) is not used. The documents' "/" on a negative
 *     number is taken as rounding down (an arithmetic shift, as with the hardware dropping the low 3 bits of the table's values:
 *     the document says the chip uses the upper 13 bits). The document says the hardware's results are not perfect, so these
 *     are not claimed to be the chip's.
 *   - THE RUN-LEVEL STEP divides (level * quant * scale + 4) by 8 rounding down. A block is finished when its 64th coefficient
 *     has been stored; the end-of-block code that follows is then skipped at the start of the next block (the document: FE00h
 *     "at the begin of a new block, or after the end of block ... is simply ignored"). The document's pseudo-code reads one more
 *     halfword in that case; the two agree whenever the end-of-block code is there, which a bitstream from port_mdec_vlc always has.
 *   - THE COLOUR CONVERSION: the document gives the constants as decimals (1.402, 1.772, -0.3437, -0.7143) and says the fixed
 *     point resolution is unknown. This is THIS PROJECT'S choice, not the chip's known arithmetic: each constant scaled by 65536
 *     and rounded to an integer (91881, 116131, -22525, -46812); a component is Y + ((k1 * Cr + k2 * Cb + 32768) >> 16), rounding
 *     down at the half; then the document's saturation to -128..127; then + 128 (an xor with 80h) when the output is unsigned.
 *     The 24 bit output is three bytes per pixel in the order red, green, blue. The 15 bit output is a 16 bit little-endian value:
 *     red in bits 0 to 4, green 5 to 9, blue 10 to 14, each the top 5 bits of the 8 bit value (the low 3 bits dropped), and bit 15
 *     the mask bit when asked. The document says none of this about the 15 bit form; it is a reading.
 *   - THE QUANTISATION TABLES AT RESET are the default intra quantiser matrix of the MPEG-1 video standard (ISO/IEC 11172-2) with
 *     its first entry replaced by 2, the same matrix for both tables, stored in the zigzag order in which the chip's command takes
 *     its 64 bytes and the decoder indexes them. The documents do NOT give these values. What the chapter on video streams does
 *     say is that the library's default table begins 02h, 10h, 10h, 13h and ends 53h (83), which this matrix does, and the first
 *     entry 2 and the use of one matrix for both tables are NOT from the documents. The check against the library is a search for
 *     the 64 bytes in a program of the game (see the README).
 *   - THE SCALE TABLE at reset is the document's (the sign-and-magnitude layout it gives in hex).
 *
 * Left out: the 4 bit and 8 bit monochrome depths of the chip (port_mdec_decode refuses them); the versions of the bitstream
 * other than 1, 2 and 3 (the encrypted forms, and the forms of single games); the decoding of the stream's sectors.
 */
#ifndef PORT_MDEC_H
#define PORT_MDEC_H

/* Results. Zero is success; all failures are negative. */
enum {
    PORT_MDEC_OK = 0,
    PORT_MDEC_E_HEADER = -1,     /* port_mdec_vlc: not a frame's header (too short, file id not 3800h, quantisation above 3Fh) */
    PORT_MDEC_E_VERSION = -2,    /* port_mdec_vlc: a version that is not 1, 2 or 3 */
    PORT_MDEC_E_CODE = -3,       /* port_mdec_vlc: a code that is not in the table (or a raw value equal to FE00h) */
    PORT_MDEC_E_TRUNC = -4,      /* port_mdec_vlc: the bitstream ends inside a block, or without an end-of-frame code */
    PORT_MDEC_E_ROOM = -5,       /* the output is too small (the codes, or the pixels) */
    PORT_MDEC_E_INCOMPLETE = -6, /* port_mdec_decode: the codes end inside a macroblock */
    PORT_MDEC_E_DEPTH = -7,      /* port_mdec_decode: a depth other than 24 and 15 */
    PORT_MDEC_E_ARG = -8,        /* a null pointer where a size above 0 (or a result) is needed */
    PORT_MDEC_E_RUN = -9,        /* port_mdec_vlc: a run that goes past the 64th coefficient of a block */
    PORT_MDEC_E_COMMAND = -10,   /* a setter: the command word is not the command, or the parameters are too short */
};

/* One frame's bitstream (its header first) to the chip's run-level codes, as the library routine DecDCTvlc hands them on.
 *
 * `frame` is `bytes` long: the 8 byte header (halfwords, little-endian: size/4, file id 3800h, quantisation 0 to 3Fh, version 1
 * 2 or 3), then the bitstream, read as little-endian 16 bit units with the first bit in bit 15 of each unit. A last odd byte is
 * not read. The size field is not used.
 *
 * `codes` has room for `room` 16 bit codes. For each block, in the order Cr, Cb, Y1, Y2, Y3, Y4 of each macroblock, are written: the
 * first code (the quantisation scale in bits 10 to 15 and the DC value in bits 0 to 9), the codes of the run-level pairs (an
 * escape's 16 bit value as it is), and the end-of-block code FE00h. In version 3 the DC is the running sum of the offsets, per
 * component (Cr, Cb, and one for the four Y blocks), wrapped to 10 bits (the document's newer reading). The frame ends with the
 * end-of-frame code (a Cr position whose DC is 1FFh in versions 1 and 2, ten 1-bits in version 3); it writes nothing. No padding
 * halfwords are written.
 *
 * `*used` is set to the number of codes written, on success and on failure. Returns 0 or a negative number (the enum above). It
 * never reads beyond `bytes` of `frame` and never writes beyond `room` codes of `codes`. */
int port_mdec_vlc(const unsigned char *frame, unsigned bytes, unsigned short *codes, unsigned room, unsigned *used);

/* The decoder's tables. `iq_y` (used for Y1..Y4) and `iq_uv` (Cr and Cb) are the quantisation tables in zigzag order, as the 64
 * bytes of the chip's second command give them; `scale` is the 64 signed halfwords of its third command (14 fractional bits). */
struct port_mdec {
    unsigned char iq_y[64];
    unsigned char iq_uv[64];
    short scale[64];
};

/* Sets the tables the library sets at its reset (see above). */
void port_mdec_reset(struct port_mdec *m);

/* The chip's second command: `command` is its word (bits 31 to 29 must be 2; bit 0 set means that a second table follows),
 * `bytes` its parameters: 64 bytes for the luminance table, and 64 more for the colour table when bit 0 is set. `size` is the number
 * of bytes at `bytes`; fewer than needed leaves the tables unchanged and returns PORT_MDEC_E_COMMAND, as does a wrong command. */
int port_mdec_set_quant(struct port_mdec *m, unsigned command, const unsigned char *bytes, unsigned size);

/* The chip's third command: `command` (bits 31 to 29 must be 3), `bytes` the 64 signed halfwords, 128 bytes, little-endian.
 * Fewer than 128 bytes or a wrong command leaves the table unchanged and returns PORT_MDEC_E_COMMAND. */
int port_mdec_set_scale(struct port_mdec *m, unsigned command, const unsigned char *bytes, unsigned size);

/* Run-level codes to pixels, as the chip's decode command produces them: macroblock after macroblock, each of the six blocks
 * Cr, Cb, Y1, Y2, Y3, Y4 (Y1 upper left, Y2 upper right, Y3 lower left, Y4 lower right), each macroblock 16 by 16 pixels in raster order
 * (16 pixels to a row), 3 bytes a pixel for `depth` 24, 2 bytes for `depth` 15 (the mask bit, bit 15 of the pixel, set when
 * `set_mask`). `is_signed` leaves the pixels signed (-128..127, as bytes), otherwise 128 is added (the usual case).
 *
 * `codes` is `count` halfwords. An FE00h at the beginning of a block is skipped (padding); a trailing run of them after the last
 * block is accepted. `pixels` has room for `room` bytes. `*blocks` is set to the number of macroblocks written and `*written` to
 * the number of bytes, on success and on failure (the output holds the macroblocks completed before the failure). Returns 0, or
 * PORT_MDEC_E_INCOMPLETE (the codes end inside a macroblock), PORT_MDEC_E_ROOM (the next macroblock does not fit),
 * PORT_MDEC_E_DEPTH (a depth that is not 24 or 15; the 4 and 8 bit depths of the chip are left out), PORT_MDEC_E_ARG. It never
 * reads beyond `count` codes and never writes beyond `room` bytes. */
int port_mdec_decode(const struct port_mdec *m, const unsigned short *codes, unsigned count, int depth, int set_mask, int is_signed,
                     unsigned char *pixels, unsigned room, unsigned *blocks, unsigned *written);

#endif
