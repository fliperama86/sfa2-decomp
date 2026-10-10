/* The decoder of the disc's streamed audio. See xa.h for the format, its source and what is not the console's. */
#include "xa.h"

#include <string.h>

static const int coef0[4] = { 0, 60, 115, 98 };
static const int coef1[4] = { 0, 0, -52, -55 };

void xa_reset(struct xa_state *s)
{
    memset(s, 0, sizeof *s);
}

int xa_parse(const uint8_t *raw, struct xa_info *info)
{
    if (memcmp(raw + 16, raw + 20, 4) != 0) return XA_BAD_SUBHEADER;
    info->file = raw[16];
    info->channel = raw[17];
    info->submode = raw[18];
    info->coding = raw[19];
    if (!(info->submode & 0x04)) return XA_NOT_AUDIO;
    info->stereo = info->coding & 1;
    info->rate = (info->coding & 4) ? 18900 : 37800;
    info->bits = (info->coding & 0x10) ? 8 : 4;
    info->frames = info->stereo ? 18u * 4u * 28u : 18u * 8u * 28u;
    if (info->bits == 8) return XA_EIGHT_BIT;
    return XA_OK;
}

static int clamp16(int v)
{
    return v < -32768 ? -32768 : v > 32767 ? 32767 : v;
}

/* v >> 6 with the result rounded down for a negative v, whatever the compiler's shift of a negative value does. */
static int floor64(int v)
{
    return v >= 0 ? v / 64 : -((-v + 63) / 64);
}

int xa_decode(struct xa_state *s, const uint8_t *raw, int16_t *out, struct xa_info *info)
{
    int hist[2][2], g, b, j, r = xa_parse(raw, info);
    unsigned frame = 0;

    if (r != XA_OK) return r;
    memcpy(hist, s->hist, sizeof hist);
    for (g = 0; g < 18; g++) {
        const uint8_t *grp = raw + 24 + g * 128;
        for (b = 0; b < 8; b++) {
            int head = grp[4 + b], range = head & 15, filter = (head >> 4) & 3;
            int shift = 12 - (range > 12 ? 9 : range);
            int ch = info->stereo ? (b & 1) : 0;
            for (j = 0; j < 28; j++) {
                int nib = (grp[16 + (b >> 1) + 4 * j] >> (4 * (b & 1))) & 15;
                int t = nib >= 8 ? nib - 16 : nib;
                int v = clamp16(t * (1 << shift) + floor64(hist[ch][0] * coef0[filter] + hist[ch][1] * coef1[filter] + 32));
                unsigned at = (frame + (unsigned)j) * 2;
                hist[ch][1] = hist[ch][0];
                hist[ch][0] = v;
                if (info->stereo) {
                    out[at + (unsigned)ch] = (int16_t)v;
                } else {
                    out[at] = out[at + 1] = (int16_t)v;
                }
            }
            if (!info->stereo || (b & 1)) frame += 28;
        }
    }
    memcpy(s->hist, hist, sizeof hist);
    return XA_OK;
}

unsigned xa_resampled_bound(unsigned n, unsigned rate)
{
    return (unsigned)(((unsigned long long)n * XA_OUT_RATE) / rate) + 2u;
}

/* Output k sits at source position phase_k / 44100 after the frame `prev` (frame 0 of this call is at position 1). It exists once
 * the frame after it is known. The value is a + (b - a) * frac / 44100, the division truncating toward zero. */
unsigned xa_resample(struct xa_state *s, const int16_t *src, unsigned n, unsigned rate, int16_t *dst)
{
    unsigned count = 0;
    uint32_t phase;

    if (!n) return 0;
    if (!s->have_prev) {
        s->have_prev = 1;
        s->prev[0] = src[0];
        s->prev[1] = src[1];
        s->phase = 0;   /* the first output is the first source frame itself */
    }
    phase = s->phase;
    for (;;) {
        unsigned idx = phase / XA_OUT_RATE;      /* index of the frame at or before: 0 is prev, k is src[k - 1] */
        uint32_t frac = phase % XA_OUT_RATE;
        int c;
        if (idx + 1 > n) break;                  /* needs src[idx] as the later frame */
        for (c = 0; c < 2; c++) {
            int a = idx == 0 ? s->prev[c] : src[(idx - 1) * 2 + (unsigned)c];
            int bb = src[idx * 2 + (unsigned)c];
            dst[count * 2 + (unsigned)c] = (int16_t)(a + (int)(((long long)(bb - a) * (long long)frac) / (long long)XA_OUT_RATE));
        }
        count++;
        phase += rate;
    }
    s->phase = phase - n * XA_OUT_RATE;
    s->prev[0] = src[(n - 1) * 2];
    s->prev[1] = src[(n - 1) * 2 + 1];
    return count;
}
