/* The PS1's sound samples (the body of a VAB: SPU ADPCM, 16-byte blocks) built from the arcade's QSound samples, and a reader for the
 * Z80 sound program's instrument table.
 *
 * What samples.py measured on 2026-10-23 and this file reproduces (samples.py has the model; `samples.py --compare-c` checks this C
 * against it and against the disc): a PS1 sample is an arcade sample resampled from 24,000 Hz to a rate R of the PS1 team's choice
 * (one of 11), times 256, SPU ADPCM encoded. Block 0 is zeros, block 1 starts the data (flags 4), the last data block has flags 1 and a
 * closing block (flags 7, filter 0, shift 0, nibbles 7) ends it. The sample holds ceil(length * R / (24000 * 28)) + 2 blocks for an
 * arcade sample of `length` data. The PS1's own encoder was coarser (17 dB against the arcade data; this one 29 dB), so the bytes are
 * not the disc's, the sizes are. Not built here: the loop samples and the samples with no arcade source (the PS1's own).
 *
 * The arcade's instrument table is in the Z80 program (sz2.01a, sz2.02a): 435 records of 8 bytes at 0x8000 of the sound view, one per
 * QSound sample: a bank byte (64 KB banks of the sample ROM), the start, the loop start and the (inclusive) end as 16-bit addresses
 * in that bank, and the root key the sample is at its natural pitch for (60 or 72, 75 once). It has no rate, volume or envelope.
 *
 * Project-authored, plain C99. No byte of either game is here. */
#ifndef ARCADE_VAG_H
#define ARCADE_VAG_H

#include <stddef.h>
#include <stdint.h>

#define ARCADE_VAG_ARCADE_RATE 24000u
#define ARCADE_VAG_BLOCK_SAMPLES 28u
#define ARCADE_VAG_BLOCK_BYTES 16u
#define ARCADE_VAG_MIN_RATE 4000u
#define ARCADE_VAG_MAX_RATE 48000u
#define ARCADE_INSTRUMENT_COUNT 435u
#define ARCADE_INSTRUMENT_TABLE 0x8000u   /* offset in the sound view (the Z80's first banked window) */
#define ARCADE_INSTRUMENT_SIZE 8u

struct arcade_instrument {
    uint32_t start;    /* address in the sample ROM (the chip's view): bank * 0x10000 + start */
    uint32_t loop;     /* the same for the loop start */
    uint32_t end;      /* the same for the last datum (inclusive) */
    uint8_t root;      /* the key at which the sample plays at its natural pitch */
};

/* Record `index` of the instrument table of the Z80 program (`size` bytes). Returns 0, or -1 when index is out of range, the view is
 * too small, the bank is past the 4 MB of samples or the addresses are not in order (start <= loop <= end). */
int arcade_instrument(const uint8_t *sound, size_t size, unsigned index, struct arcade_instrument *out);

/* Blocks of a sample made from `length` arcade data at `rate` Hz, by the rule above; 0 on a bad argument. */
uint32_t arcade_vag_blocks(uint32_t length, uint32_t rate);

/* Build the sample into out[0..room): `blocks` blocks (at least 4) made from samples[start .. start + length) (signed 8-bit, the chip's
 * order; what lies outside that range counts as silence), resampled to `rate`. `blocks` is the rule's count, or the disc's where a sample
 * was cut or lengthened. Returns 0, or -1 with a line in err (nothing is written past room, and out is untouched on a bad argument).
 * Not thread-safe the first time (it makes the resampler's table). */
int arcade_vag_encode(const uint8_t *samples, size_t size, uint32_t start, uint32_t length, uint32_t rate, uint32_t blocks,
                      uint8_t *out, size_t room, char *err, size_t errsize);

/* The SPU ADPCM decoder, for tests and checks: decodes up to `count` values of `size` bytes of blocks (a multiple of 16 is used, the rest
 * ignored) into out and stores the number made in *made. If flags is not NULL, flags[block] is each block's flag byte (room for size / 16). */
void arcade_vag_decode(const uint8_t *vag, size_t size, int16_t *out, size_t count, size_t *made, uint8_t *flags);

/* The generated table (vag_table.c, from samples.py c-table): one row per distinct sample of the disc's banks, and the banks. */
#define ARCADE_VAG_NONE 0xffffu
struct arcade_vag_sample {
    uint16_t instrument;   /* record of the Z80 instrument table the sample starts at; ARCADE_VAG_NONE when the arcade has no source */
    uint16_t rate;         /* Hz the PS1 resampled it to */
    uint16_t blocks;       /* its size in blocks, the disc's */
    uint16_t flags;        /* bit 0: the block count is not the rule's (the PS1 cut or lengthened the sample) */
};
struct arcade_vag_bank {
    const char *file;      /* disc archive name without ".PAC", for instance "PL00" */
    uint8_t slot;          /* the VAB's slot in the archive (table 3 slot s and table 4 slot s) */
    uint8_t count;         /* VAGs */
    uint16_t first;        /* index of its first sample number in the table's list */
};
extern const struct arcade_vag_sample arcade_vag_samples[];
extern const unsigned arcade_vag_sample_count;
extern const struct arcade_vag_bank arcade_vag_banks[];
extern const unsigned arcade_vag_bank_count;
/* The sample numbers (into arcade_vag_samples) of a bank's VAGs 1 .. count. */
const uint16_t *arcade_vag_bank_ids(const struct arcade_vag_bank *bank);
/* The bank of an archive's slot, or NULL. */
const struct arcade_vag_bank *arcade_vag_find_bank(const char *file, unsigned slot);

/* Build sample number `sample` of the table from the arcade's views (the sample ROM in the chip's order, the Z80 program): its blocks into
 * out[0..room), *size = their bytes (the disc's size). Returns 0, or -1 with a line in err (also for a sample with no arcade source). */
int arcade_vag_build(const uint8_t *samples, size_t samples_size, const uint8_t *sound, size_t sound_size, unsigned sample, uint8_t *out,
                     size_t room, size_t *size, char *err, size_t errsize);

#endif
