/* The PS1's sound samples built from the arcade's QSound samples: see vag.h. The Python model in samples.py (resample_table, encode_block,
 * vag_model) is the specification; the arithmetic here is the same step for step (doubles for the resampler, integers for the coder),
 * so that the two agree byte for byte. */
#include "vag.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#define KERNEL_RADIUS 16
#define KERNEL_OVERSAMPLE 512
#define KERNEL_SIZE (KERNEL_RADIUS * KERNEL_OVERSAMPLE + 2)
#define ARCADE_SAMPLES_TOTAL 0x400000u

static const double PI = 3.14159265358979323846;
static const int FILTER_A[5] = {0, 60, 115, 98, 122};
static const int FILTER_C[5] = {0, 0, -52, -55, -60};

static void fail(char *err, size_t errsize, const char *msg) {
    if (err && errsize) {
        snprintf(err, errsize, "%s", msg);
    }
}

int arcade_instrument(const uint8_t *sound, size_t size, unsigned index, struct arcade_instrument *out) {
    const uint8_t *r;
    uint32_t bank;
    uint32_t start, loop, end;
    if (!sound || !out || index >= ARCADE_INSTRUMENT_COUNT) {
        return -1;
    }
    if (size < ARCADE_INSTRUMENT_TABLE + (size_t)ARCADE_INSTRUMENT_SIZE * (index + 1u)) {
        return -1;
    }
    r = sound + ARCADE_INSTRUMENT_TABLE + (size_t)ARCADE_INSTRUMENT_SIZE * index;
    bank = r[0];
    if (bank >= ARCADE_SAMPLES_TOTAL / 0x10000u) {
        return -1;
    }
    start = (uint32_t)r[1] | ((uint32_t)r[2] << 8);
    loop = (uint32_t)r[3] | ((uint32_t)r[4] << 8);
    end = (uint32_t)r[5] | ((uint32_t)r[6] << 8);
    if (!(start <= loop && loop <= end)) {
        return -1;
    }
    out->start = (bank << 16) | start;
    out->loop = (bank << 16) | loop;
    out->end = (bank << 16) | end;
    out->root = r[7];
    return 0;
}

uint32_t arcade_vag_blocks(uint32_t length, uint32_t rate) {
    uint64_t num, den;
    if (length == 0 || rate < ARCADE_VAG_MIN_RATE || rate > ARCADE_VAG_MAX_RATE) {
        return 0;
    }
    num = (uint64_t)length * rate;
    den = (uint64_t)ARCADE_VAG_ARCADE_RATE * ARCADE_VAG_BLOCK_SAMPLES;
    return (uint32_t)((num + den - 1u) / den) + 2u;
}

/* --- the resampler: a Lanczos kernel of radius 16 from a table with linear interpolation, band-limited when it takes fewer samples --- */

static double kernel_table[KERNEL_SIZE];
static int kernel_ready;

static void make_kernel(void) {
    int i;
    for (i = 0; i < KERNEL_SIZE; i++) {
        double x = (double)i / KERNEL_OVERSAMPLE;
        if (i == 0) {
            kernel_table[i] = 1.0;
        } else if (x >= KERNEL_RADIUS) {
            kernel_table[i] = 0.0;
        } else {
            double px = PI * x;
            kernel_table[i] = sin(px) / px * sin(px / KERNEL_RADIUS) / (px / KERNEL_RADIUS);
        }
    }
    kernel_ready = 1;
}

static double kernel(double x) {
    double t, f;
    int i;
    if (x < 0) {
        x = -x;
    }
    if (x >= KERNEL_RADIUS) {
        return 0.0;
    }
    t = x * KERNEL_OVERSAMPLE;
    i = (int)t;
    f = t - i;
    return kernel_table[i] * (1.0 - f) + kernel_table[i + 1] * f;
}

/* The value at position p of the samples src[0..length) (signed 8-bit), zero outside; rho is the step per output sample. */
static double resample_at(const uint8_t *src, uint32_t length, double p, double cut, int half) {
    double s = 0.0;
    long i0 = (long)floor(p);
    long j;
    for (j = i0 - half + 1; j <= i0 + half; j++) {
        if (j >= 0 && j < (long)length) {
            s += (double)(int8_t)src[j] * kernel(((double)p - (double)j) * cut) * cut;
        }
    }
    return s;
}

/* --- the coder --- */

/* t >> s with the rounding of an arithmetic shift (down), without relying on the compiler's. */
static int sar(int t, int s) {
    return t >= 0 ? t >> s : -((-t + (1 << s) - 1) >> s);
}

/* Closed-loop encode of 28 values: the (filter, shift) of least squared error; h1, h2 are the decoder's last two outputs. */
static void encode_block(const int x[28], int *h1, int *h2, uint8_t out[16]) {
    int have = 0;
    int64_t best_err = 0;
    int best_f = 0, best_shift = 0, best_p1 = 0, best_p2 = 0;
    int best_nib[28];
    int f, shift, k;
    for (f = 0; f < 5; f++) {
        for (shift = 0; shift < 13; shift++) {
            int step = 4096 >> shift;
            int p1 = *h1, p2 = *h2;
            int64_t err = 0;
            int nib[28];
            int broke = 0;
            for (k = 0; k < 28; k++) {
                int v = x[k];
                int pred = sar(p1 * FILTER_A[f] + p2 * FILTER_C[f] + 32, 6);
                long n = lrint((double)(v - pred) / step);
                int y;
                if (n < -8) {
                    n = -8;
                } else if (n > 7) {
                    n = 7;
                }
                y = (int)n * step + pred;
                if (y < -32768) {
                    y = -32768;
                } else if (y > 32767) {
                    y = 32767;
                }
                err += (int64_t)(v - y) * (v - y);
                if (have && err >= best_err) {
                    broke = 1;
                    break;
                }
                nib[k] = (int)(n & 15);
                p2 = p1;
                p1 = y;
            }
            if (!broke && (!have || err < best_err)) {
                have = 1;
                best_err = err;
                best_f = f;
                best_shift = shift;
                best_p1 = p1;
                best_p2 = p2;
                memcpy(best_nib, nib, sizeof nib);
            }
        }
    }
    out[0] = (uint8_t)((best_f << 4) | best_shift);
    out[1] = 0;
    for (k = 0; k < 28; k += 2) {
        out[2 + k / 2] = (uint8_t)(best_nib[k] | (best_nib[k + 1] << 4));
    }
    *h1 = best_p1;
    *h2 = best_p2;
}

int arcade_vag_encode(const uint8_t *samples, size_t size, uint32_t start, uint32_t length, uint32_t rate, uint32_t blocks,
                      uint8_t *out, size_t room, char *err, size_t errsize) {
    double rho, cut;
    int half;
    uint32_t b, k;
    int h1 = 0, h2 = 0;
    if (!samples || !out) {
        fail(err, errsize, "vag: no samples or no room");
        return -1;
    }
    if (rate < ARCADE_VAG_MIN_RATE || rate > ARCADE_VAG_MAX_RATE) {
        fail(err, errsize, "vag: rate out of range");
        return -1;
    }
    if (blocks < 4 || blocks > 0x100000u) {
        fail(err, errsize, "vag: block count out of range");
        return -1;
    }
    if (room < (size_t)blocks * ARCADE_VAG_BLOCK_BYTES) {
        fail(err, errsize, "vag: room too small");
        return -1;
    }
    if (start >= size || length == 0) {
        fail(err, errsize, "vag: start or length outside the samples");
        return -1;
    }
    if (length > size - start) {
        length = (uint32_t)(size - start);
    }
    if (!kernel_ready) {
        make_kernel();
    }
    rho = (double)ARCADE_VAG_ARCADE_RATE / (double)rate;
    cut = rho > 1.0 ? 1.0 / rho : 1.0;
    half = (int)ceil(KERNEL_RADIUS / cut);
    memset(out, 0, ARCADE_VAG_BLOCK_BYTES);
    for (b = 1; b + 1 < blocks; b++) {
        int x[28];
        for (k = 0; k < 28; k++) {
            double p = rho * (double)((b - 1u) * 28u + k);
            double v = 256.0 * resample_at(samples + start, length, p, cut, half);
            long r = lrint(v);
            x[k] = r < -32768 ? -32768 : r > 32767 ? 32767 : (int)r;
        }
        encode_block(x, &h1, &h2, out + (size_t)b * ARCADE_VAG_BLOCK_BYTES);
    }
    out[ARCADE_VAG_BLOCK_BYTES + 1] |= 4;
    out[(size_t)(blocks - 2) * ARCADE_VAG_BLOCK_BYTES + 1] |= 1;
    out += (size_t)(blocks - 1) * ARCADE_VAG_BLOCK_BYTES;
    out[0] = 0;
    out[1] = 7;
    memset(out + 2, 0x77, 14);
    return 0;
}

void arcade_vag_decode(const uint8_t *vag, size_t size, int16_t *out, size_t count, size_t *made, uint8_t *flags) {
    size_t i, n = 0, blk = 0;
    int h1 = 0, h2 = 0;
    for (i = 0; i + 16 <= size; i += 16, blk++) {
        int shift = vag[i] & 15;
        int filt = vag[i] >> 4;
        int k;
        if (shift > 12) {
            shift = 9;
        }
        if (filt > 4) {
            filt = 4;
        }
        if (flags) {
            flags[blk] = vag[i + 1];
        }
        for (k = 0; k < 28 && n < count; k++) {
            int nib = (vag[i + 2 + k / 2] >> (4 * (k & 1))) & 15;
            int s;
            if (nib >= 8) {
                nib -= 16;
            }
            s = sar(nib * 4096, shift) + sar(h1 * FILTER_A[filt] + h2 * FILTER_C[filt] + 32, 6);
            s = s < -32768 ? -32768 : s > 32767 ? 32767 : s;
            out[n++] = (int16_t)s;
            h2 = h1;
            h1 = s;
        }
    }
    if (made) {
        *made = n;
    }
}

const struct arcade_vag_bank *arcade_vag_find_bank(const char *file, unsigned slot) {
    unsigned i;
    if (!file) {
        return NULL;
    }
    for (i = 0; i < arcade_vag_bank_count; i++) {
        if (arcade_vag_banks[i].slot == slot && strcmp(arcade_vag_banks[i].file, file) == 0) {
            return &arcade_vag_banks[i];
        }
    }
    return NULL;
}

int arcade_vag_build(const uint8_t *samples, size_t samples_size, const uint8_t *sound, size_t sound_size, unsigned sample, uint8_t *out,
                     size_t room, size_t *size, char *err, size_t errsize) {
    const struct arcade_vag_sample *s;
    struct arcade_instrument inst;
    if (sample >= arcade_vag_sample_count) {
        fail(err, errsize, "vag: no such sample");
        return -1;
    }
    s = &arcade_vag_samples[sample];
    if (s->instrument == ARCADE_VAG_NONE) {
        fail(err, errsize, "vag: the arcade has no source for this sample");
        return -1;
    }
    if (arcade_instrument(sound, sound_size, s->instrument, &inst)) {
        fail(err, errsize, "vag: the instrument table of the sound program is not readable");
        return -1;
    }
    if (arcade_vag_encode(samples, samples_size, inst.start, inst.end - inst.start + 1u, s->rate, s->blocks, out, room, err, errsize)) {
        return -1;
    }
    if (size) {
        *size = (size_t)s->blocks * ARCADE_VAG_BLOCK_BYTES;
    }
    return 0;
}
