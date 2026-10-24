/* The arcade's music and sound-effect player (see music.h). The driver half follows port/arcade/music.py statement by statement: the same
 * 64-byte track states, the same order of work, the same register writes. The mixer half is the chip. Project-authored C99; no byte of
 * either game is here. */
#include <stdlib.h>
#include <string.h>

#include "music.h"

/* ---- addresses in the Z80 program (music.md section 1) ---- */
#define INSTR_TABLE 0x8000u
#define CMD_TABLE 0x9000u
#define PROG_TABLE 0x7000u
#define PROG_TABLE2 0x7800u
#define T_DURATION_NORMAL 0x0d36u
#define T_DURATION_BINARY 0x0d2fu
#define T_DURATION_DOTTED 0x0d3du
#define T_OCTAVE 0x0d44u
#define T_VOLUME 0x25e9u
#define T_LEVEL 0x26e9u
#define T_ATTACK 0x27e9u
#define T_RATE 0x2869u
#define T_LFO_RATE 0x28e9u
#define T_VIB_DEPTH 0x29e9u
#define T_TREM_DEPTH 0x2ae9u
#define T_OCT_OF_KEY 0x1542u
#define T_OCT_BASE 0x15aeu
#define T_FINE 0x1767u
#define T_FREQ_SFX 0x15b7u
#define T_FREQ_MUS 0x168fu

/* offsets in a track's state */
#define FLAGS 3
#define DUR 4
#define GATE_RATIO 6
#define GATE_T 7
#define TRANSPOSE 9
#define FINE 10
#define ROOT 11
#define INS_FINE 12
#define VOL 13
#define ECHO 15
#define SLIDE 16
#define OSC 23
#define RUNNING 32
#define STAGE 37
#define LOOPS 44
#define INSTR 48
#define PRIORITY 52
#define SILENT 53
#define PANMODE 54
#define PAN 55

/* ---- mixer constants: the guesses (music.h) ---- */
#define CHIP_CLOCK_HALF 30000000u          /* 60 MHz / 2 */
#define CHIP_DIVIDER 1248u                 /* the chip's sample rate is CHIP_CLOCK_HALF / CHIP_DIVIDER = 24,038 Hz */
#define VOLUME_UNITY 8192.0f               /* register 6 = 0x2000 plays the 8-bit sample at full scale */
#define PAN_REG_BASE 0x110                 /* register 0x80 + voice = 0x110 + p, p = 0 (left) .. 32 (right) */
#define ECHO_LENGTH_BASE 0x554u            /* the delay in chip samples is register 0xd9 minus this */
#define ECHO_MAX 4096u                     /* frames of the delay line */
#define ECHO_FEEDBACK_MAX 0.95f            /* keeps a mis-set feedback register from running away */
#define OUTPUT_GAIN 0.6f                   /* what is left of the sum before the 16-bit clip */
#define SAMPLE_SCALE 256.0f                /* 8-bit sample to 16-bit */
#define FIRST_PASS_MASK 3u                 /* a pass is every fourth interrupt */
#define MAX_COMMANDS 200000u               /* bytes a track may take in one call before it is stopped as broken */

static const float pan_sin[33] = {
    0.000000f, 0.049068f, 0.098017f, 0.146730f, 0.195090f, 0.242980f, 0.290285f, 0.336890f, 0.382683f, 0.427555f, 0.471397f,
    0.514103f, 0.555570f, 0.595699f, 0.634393f, 0.671559f, 0.707107f, 0.740951f, 0.773010f, 0.803208f, 0.831470f, 0.857729f,
    0.881921f, 0.903989f, 0.923880f, 0.941544f, 0.956940f, 0.970031f, 0.980785f, 0.989177f, 0.995185f, 0.998795f, 1.000000f};

struct chip_voice {
    uint8_t bank;
    uint16_t loop, end, rate, volume, send;
    uint8_t pan;
    uint64_t pos;       /* address in the high 32 bits (16 used), fraction in the low 32 */
    uint64_t step;      /* a frame's advance, 32 fraction bits */
};

struct arcade_music {
    const uint8_t *d;
    const uint8_t *smp;
    int stereo;

    uint8_t sfx[16][64], mus[16][64];
    uint32_t irq, tick;
    int music, sfx_on, fault;
    unsigned tempo, song_tempo, tempo_bend, transpose, pitch_bend;
    unsigned vol_scale, master_scale, song_vol;
    unsigned fade_mode, fade_level, fade_rate, fade_target;
    unsigned gfade_mode, gfade_level, gfade_rate;
    unsigned eff_prio;
    uint8_t iy[16];

    struct arcade_music_write *log;
    size_t log_cap, log_count;

    struct chip_voice cv[16];
    unsigned echo_fb, echo_len;
    float echo_l[ECHO_MAX], echo_r[ECHO_MAX];
    unsigned echo_pos;
    uint32_t tick_acc;
    float song_gain;   /* the host's gain of the music's voices (a voice no effect runs on), 1 for none; not the driver's */
};

/* ---- the chip ---- */
static void chip_step(struct chip_voice *c) {
    c->step = (uint64_t)c->rate * 1048576u * CHIP_CLOCK_HALF / ((uint64_t)CHIP_DIVIDER * ARCADE_MUSIC_RATE);
}

static void chip_write(struct arcade_music *m, unsigned reg, unsigned val) {
    val &= 0xffffu;
    if (reg < 0x80u) {
        unsigned idx = reg & 7u, n = reg >> 3;
        struct chip_voice *c = &m->cv[n];
        switch (idx) {
        case 0:
            m->cv[(n + 1u) & 15u].bank = (uint8_t)(val & 0x3fu);   /* the chip files the bank one slot early */
            break;
        case 1:
            c->pos = (uint64_t)val << 32 | (c->pos & 0xffffffffu);
            break;
        case 2:
            c->rate = (uint16_t)val;
            chip_step(c);
            break;
        case 3:
            c->pos = (c->pos & ~(uint64_t)0xffffffffu) | (uint64_t)val << 16;
            break;
        case 4:
            c->loop = (uint16_t)val;
            break;
        case 5:
            c->end = (uint16_t)val;
            break;
        case 6:
            c->volume = (uint16_t)val;
            break;
        default:
            break;
        }
    } else if (reg >= 0x80u && reg < 0x90u) {
        int p = (int)val - PAN_REG_BASE;
        m->cv[reg - 0x80u].pan = (uint8_t)(p < 0 ? 0 : p > 32 ? 32 : p);
    } else if (reg >= 0xbau && reg < 0xcau) {
        m->cv[reg - 0xbau].send = (uint16_t)val;
    } else if (reg == 0x93u) {
        m->echo_fb = val;
    } else if (reg == 0xd9u) {
        m->echo_len = val;
    }
}

static void wr(struct arcade_music *m, unsigned reg, unsigned val) {
    if (m->log) {
        if (m->log_count < m->log_cap) {
            m->log[m->log_count].tick = m->tick;
            m->log[m->log_count].reg = (uint16_t)reg;
            m->log[m->log_count].value = (uint16_t)(val & 0xffffu);
        }
    }
    m->log_count++;
    chip_write(m, reg, val);
}

/* ---- reading the Z80 program ---- */
static unsigned rb(struct arcade_music *m, size_t a) {
    if (a >= ARCADE_SOUND_SIZE) {
        m->fault = 1;
        return 0;
    }
    return m->d[a];
}

static unsigned tab(struct arcade_music *m, unsigned base, unsigned i) {
    return rb(m, base + 2u * i) | rb(m, base + 2u * i + 1u) << 8;
}

static unsigned mul16(unsigned a, unsigned b) {
    return (unsigned)(((uint32_t)a * (uint32_t)b) >> 16);
}

static unsigned neg16(unsigned x) {
    return (0u - x) & 0xffffu;
}

static unsigned ptr_get(const uint8_t *t) {
    return (unsigned)t[0] | (unsigned)t[1] << 8 | (unsigned)t[2] << 16;
}

static void ptr_set(uint8_t *t, unsigned v) {
    t[0] = (uint8_t)v;
    t[1] = (uint8_t)(v >> 8);
    t[2] = (uint8_t)(v >> 16);
}

static unsigned rd(struct arcade_music *m, uint8_t *t) {
    unsigned p = ptr_get(t);
    unsigned v = rb(m, p);
    ptr_set(t, p + 1u);
    return v;
}

static void jump(struct arcade_music *m, uint8_t *t) {
    unsigned h = rd(m, t), l = rd(m, t);
    int off = (int)(h << 8 | l);
    if (off & 0x8000)
        off -= 0x10000;
    ptr_set(t, (unsigned)((int)ptr_get(t) + off));
}

/* ---- QSound register groups ---- */
static void key_off(struct arcade_music *m, unsigned v) {
    unsigned c;
    v &= 255u;
    wr(m, (0xbau + v) & 255u, 0);
    c = ((v << 3) | (v >> 5)) & 255u;
    wr(m, (c + 6u) & 255u, 0);
    wr(m, (c + 2u) & 255u, 0);
    wr(m, (c + 2u) & 255u, 0x1000);
}

static void pan_write(struct arcade_music *m, unsigned v, unsigned e) {
    if (!m->stereo)
        e = 0x10;
    wr(m, 0x80u + v, 0x110u + e);
}

static void adpcm_reset(struct arcade_music *m) {
    unsigned c;
    for (c = 0; c < 3; c++) {
        wr(m, 0xcdu + 4u * c, 0);
        wr(m, 0xd6u + c, 1);
    }
}

static void reset_chip(struct arcade_music *m) {
    static const unsigned regs[12][2] = {{0xde, 0},      {0xdf, 0x2e},   {0xe0, 0},      {0xe1, 0x30},   {0xe4, 0x3fff}, {0xe5, 0x3fff},
                                         {0xe6, 0x3fff}, {0xe7, 0x3fff}, {0xe2, 0xffff}, {0xda, 0x0db2}, {0xdc, 0x0e11}, {0xe3, 0x39}};
    unsigned i;
    for (i = 0; i < 12; i++)
        wr(m, regs[i][0], regs[i][1]);
    for (i = 0; i < 16; i++)
        key_off(m, i);
}

static void reset_chip_voices(struct arcade_music *m) {
    unsigned v;
    for (v = 0; v < 16; v++)
        key_off(m, v);
    adpcm_reset(m);
}

/* ---- rate of a key index (0x14fd) ---- */
static unsigned rate_of(struct arcade_music *m, unsigned idx, unsigned frac, int music) {
    unsigned octv = rb(m, T_OCT_OF_KEY + idx);
    int r = (int)idx - (int)rb(m, T_OCT_BASE + octv);
    unsigned base = 0;
    long a = (long)T_FINE + ((long)r * 256) + (long)frac;
    if (a >= 0)
        base = rb(m, (size_t)a);
    base = octv < 16 ? base << octv : 0;
    return (base + tab(m, music ? T_FREQ_MUS : T_FREQ_SFX, idx)) & 0xffffu;
}

/* ---- volume and pitch registers of a voice (0x1400) ---- */
static void voice_update(struct arcade_music *m, uint8_t *t, unsigned v, int music, int force) {
    unsigned lfo_rate, phase, depth_t, level, vol, vib, cur, h, a, x;
    if (!force && (t[SILENT] || m->sfx_on))
        return;
    lfo_rate = t[24] | (unsigned)t[25] << 8;
    phase = t[26] | (unsigned)t[27] << 8;
    depth_t = t[30] | (unsigned)t[31] << 8;
    level = t[42] | (unsigned)t[43] << 8;
    if (lfo_rate && depth_t) {
        x = (t[OSC] & 0x40) ? 0xffffu - phase : phase;
        x = mul16(x, depth_t);
        level = mul16(0xffffu - x, level);
    }
    vol = level;
    if (music) {
        vol = mul16(vol, m->song_vol);
        if (m->fade_mode)
            vol = mul16(vol, m->fade_level);
    }
    if (m->gfade_mode)
        vol = mul16(vol, m->gfade_level);
    if (m->master_scale)
        vol = mul16(vol, m->master_scale);
    vol = mul16(vol, t[13] | (unsigned)t[14] << 8);
    wr(m, v * 8u + 6u, vol);

    vib = t[28] | (unsigned)t[29] << 8;
    cur = (unsigned)t[19] << 8 | t[20];
    if (lfo_rate && vib) {
        x = (t[OSC] & 0x40) ? 0xffffu - phase : phase;
        x = mul16(x, vib);
        if (t[OSC] & 0x80)
            x = neg16(x);
        cur = (x + cur) & 0xffffu;
    }
    if (music)
        cur = (cur + m->pitch_bend) & 0xffffu;
    h = ((cur >> 8) - 1u) & 255u;
    a = (0x3cu - t[ROOT] + h) & 255u;
    if (a >= 0x6cu)
        a = ((a - 0x6cu) & 0x80u) ? 0x6bu : 0u;
    wr(m, v * 8u + 2u, rate_of(m, a, cur & 255u, music));
}

/* ---- instruments and envelope programs ---- */
static void load_instrument(struct arcade_music *m, uint8_t *t, unsigned v, unsigned id) {
    unsigned o = (INSTR_TABLE + 8u * id) & 0xffffu;
    unsigned bank = rb(m, o), start = rb(m, o + 1) | rb(m, o + 2) << 8, loop = rb(m, o + 3) | rb(m, o + 4) << 8;
    unsigned end = rb(m, o + 5) | rb(m, o + 6) << 8, root = rb(m, o + 7);
    wr(m, v * 8u + 2u, 0);
    t[34] = (uint8_t)(start >> 8);
    t[33] = (uint8_t)start;
    wr(m, v * 8u + 1u, start);
    wr(m, v * 8u + 5u, end);
    wr(m, v * 8u + 4u, (end - loop) & 0xffffu);
    wr(m, ((v - 1u) & 15u) * 8u, 0x8000u | bank);
    t[ROOT] = (uint8_t)root;
}

static void program(struct arcade_music *m, uint8_t *t, unsigned v, unsigned base, unsigned n) {
    unsigned rec = base + 8u * n, s, ident;
    t[INS_FINE] = (uint8_t)rb(m, rec + 2);
    t[35] = (uint8_t)(rec + 3u);
    t[36] = (uint8_t)((rec + 3u) >> 8);
    s = tab(m, T_LEVEL, rb(m, rec + 5));
    t[38] = (uint8_t)s;
    t[39] = (uint8_t)(s >> 8);
    ident = rb(m, rec + 1) << 8 | rb(m, rec);
    if (((unsigned)t[49] << 8 | t[48]) == ident)
        return;
    t[49] = (uint8_t)(ident >> 8);
    t[48] = (uint8_t)ident;
    if (!m->sfx_on)
        load_instrument(m, t, v, ident);
}

/* ---- effects: a voice taken back by the music, and the end of an effect ---- */
static void effect_end(struct arcade_music *m, unsigned v) {
    uint8_t *s = m->sfx[v], *t = m->mus[v];
    s[RUNNING] = 0;
    s[PRIORITY] = 0;
    key_off(m, v);
    if (t[RUNNING] == 0)
        return;
    load_instrument(m, t, v, (unsigned)t[49] << 8 | t[48]);
    pan_write(m, v, t[PAN]);
    if (t[FLAGS] & 0x80) {
        wr(m, 0x93, 0x27ff);
        wr(m, 0xd9, 0x07ff);
        wr(m, 0xbau + v, tab(m, T_VOLUME, t[ECHO]) << 2);
        wr(m, v * 8u + 1u, (unsigned)t[34] << 8 | t[33]);
        wr(m, v * 8u + 3u, 0x8000);
        m->music = 1;
        voice_update(m, t, v, 1, 1);
        m->music = 0;
    } else {
        t[SILENT] = 0xff;
    }
}

/* ---- pan motion of sound effects (0x1145) ---- */
static void pan_motion(struct arcade_music *m, uint8_t *t, unsigned v) {
    unsigned k = t[PANMODE], p, r, b, hl, h, l;
    int stop = 0;
    if (k <= 1)
        return;
    p = (unsigned)t[55] << 8 | t[56];
    r = (unsigned)t[60] << 8 | t[59];
    b = t[58];
    hl = (p + r) & 0xffffu;
    h = hl >> 8;
    l = hl & 255u;
    switch (k) {
    case 2:
        if (h >= 0x21u) {
            h = (r & 0x8000u) ? 0u : 0x20u;
            r = neg16(r);
        }
        break;
    case 3:
        if (!(r & 0x8000u)) {
            if (h >= b) {
                h = b;
                b = b > t[57] ? b - t[57] : 0u;
                r = neg16(r);
            }
        } else if (!(h > b && h < 0x21u)) {
            h = b;
            b = b + t[57] < 0x20u ? b + t[57] : 0x20u;
            r = neg16(r);
        }
        break;
    case 4:
        if (h >= b) {
            h = b;
            stop = 1;
        }
        break;
    case 5:
        if (!(h > b && h < 0x21u)) {
            h = b;
            stop = 1;
        }
        break;
    case 6:
        if (h >= b) {
            h = b;
            if (h == 0x20u) {
                stop = 1;
            } else {
                r = (unsigned)t[62] << 8 | t[61];
                b = 0x20u;
            }
        }
        break;
    case 7:
        if (!(h > b && h < 0x21u)) {
            h = b;
            if (h == 0u) {
                stop = 1;
            } else {
                r = (unsigned)t[62] << 8 | t[61];
                b = 0u;
            }
        }
        break;
    case 8:
        if (!(r & 0x8000u)) {
            if (h >= b) {
                h = b;
                stop = 1;
            }
        } else if (!(h > b && h < 0x21u)) {
            h = b;
            stop = 1;
        }
        break;
    default:
        break;
    }
    t[58] = (uint8_t)b;
    t[60] = (uint8_t)(r >> 8);
    t[59] = (uint8_t)r;
    if (stop)
        t[PANMODE] = 0;
    t[55] = (uint8_t)h;
    t[56] = (uint8_t)l;
    pan_write(m, v, h & 255u);
}

/* ---- the envelope stages (0x132c) ---- */
static void envelope(struct arcade_music *m, uint8_t *t, unsigned v) {
    unsigned st = t[STAGE], level = t[42] | (unsigned)t[43] << 8, rate = t[40] | (unsigned)t[41] << 8, nxt, a, r;
    int lv;
    if (st == 0) {
        level += rate;
        if (level <= 0xffffu) {
            t[42] = (uint8_t)level;
            t[43] = (uint8_t)(level >> 8);
            voice_update(m, t, v, m->music, 0);
            return;
        }
        nxt = 0xffffu;
    } else if (st == 1) {
        unsigned sus = t[38] | (unsigned)t[39] << 8;
        lv = (int)level - (int)rate;
        if (lv < 0 || lv - (int)sus <= 0) {
            nxt = sus;
        } else {
            level = (unsigned)lv;
            t[42] = (uint8_t)level;
            t[43] = (uint8_t)(level >> 8);
            voice_update(m, t, v, m->music, 0);
            return;
        }
    } else if (st == 3 || st == 4) {
        lv = (int)level - (int)rate;
        if (lv <= 0) {
            nxt = 0;
        } else {
            t[42] = (uint8_t)lv;
            t[43] = (uint8_t)((unsigned)lv >> 8);
            voice_update(m, t, v, m->music, 0);
            return;
        }
    } else {
        return;
    }
    if (nxt == 0) {
        a = 5;
    } else {
        a = t[STAGE] + 1u;
        if (a == 2)
            a = 3;
        r = tab(m, T_RATE, rb(m, (t[35] | (unsigned)t[36] << 8) + a));
        t[40] = (uint8_t)r;
        t[41] = (uint8_t)(r >> 8);
    }
    t[STAGE] = (uint8_t)a;
    t[43] = (uint8_t)(nxt >> 8);
    t[42] = (uint8_t)nxt;
    voice_update(m, t, v, m->music, 0);
}

/* 0x1145 .. 0x1400: pan motion, oscillator phase, pitch slide, envelope, then the voice registers */
static void after_tick(struct arcade_music *m, uint8_t *t, unsigned v) {
    unsigned ph, rate, cur, tgt, a, nw;
    int diff;
    pan_motion(m, t, v);
    ph = (t[26] | (unsigned)t[27] << 8) + (t[24] | (unsigned)t[25] << 8);
    t[26] = (uint8_t)ph;
    t[27] = (uint8_t)(ph >> 8);
    if (ph > 0xffffu)
        t[OSC] = (uint8_t)(t[OSC] + 0x40u);
    if (t[SLIDE] & 0x80) {
        rate = (unsigned)t[17] << 8 | t[18];
        cur = (unsigned)t[19] << 8 | t[20];
        tgt = (unsigned)t[21] << 8 | t[22];
        a = (rate & 0x8000u) ? 0xffu : 0u;
        nw = (cur + rate) & 0xffffu;
        diff = (int)nw - (int)tgt;
        if (diff == 0) {
            cur = tgt;
            t[SLIDE] &= 0x7f;
        } else {
            a = (a + (diff < 0 ? 1u : 0u)) & 255u;
            if (a != 0) {
                cur = nw;
            } else {
                cur = tgt;
                t[SLIDE] &= 0x7f;
            }
        }
        t[19] = (uint8_t)(cur >> 8);
        t[20] = (uint8_t)cur;
    }
    envelope(m, t, v);
}

/* ---- notes (0x0c00) ---- */
static void note(struct arcade_music *m, unsigned a, uint8_t *t, unsigned v) {
    unsigned L = (a >> 5) & 7u, fl = t[FLAGS], tb, add, dur, n = a & 0x1fu, k, pitch, sl, cur, b, g;
    int e;
    if ((fl & 0x30u) == 0) {
        tb = T_DURATION_NORMAL;
    } else if ((fl & 0x10u) == 0) {
        tb = T_DURATION_BINARY;
    } else {
        t[FLAGS] = (uint8_t)(fl & ~0x10u);
        tb = T_DURATION_DOTTED;
    }
    add = rb(m, tb + L - 1u) << 8;
    dur = ((unsigned)t[DUR] | (unsigned)t[DUR + 1] << 8) + add;
    t[DUR] = (uint8_t)dur;
    t[DUR + 1] = (uint8_t)(dur >> 8);
    if (n == 0)
        return;
    k = (n + rb(m, T_OCTAVE + (t[FLAGS] & 0x0fu))) & 255u;
    if (m->music)
        k = (k + m->transpose) & 255u;
    k = (k + t[TRANSPOSE]) & 255u;
    e = (int)((t[FINE] + t[INS_FINE]) & 255u);
    if (e >= 128)
        e -= 256;
    pitch = (unsigned)((int)(k << 8) + e) & 0xffffu;
    if ((pitch >> 8) >= 0x62u)
        pitch = 0x6100u | (pitch & 255u);
    /* portamento (0x0c2d) */
    sl = t[SLIDE] & 0x7fu;
    t[SLIDE] = (uint8_t)sl;
    cur = pitch;
    if (sl) {
        unsigned rate = (sl * 2u) & 255u, prev = t[21];
        if (prev && prev != (pitch >> 8)) {
            if (prev > (pitch >> 8))
                rate = neg16(rate);
            t[17] = (uint8_t)(rate >> 8);
            t[18] = (uint8_t)rate;
            cur = (unsigned)t[21] << 8 | t[22];
            t[SLIDE] |= 0x80;
        }
    }
    t[21] = (uint8_t)(pitch >> 8);
    t[22] = (uint8_t)pitch;
    t[19] = (uint8_t)(cur >> 8);
    t[20] = (uint8_t)cur;
    if (t[FLAGS] & 0x80) {
        t[SILENT] = 0;
        voice_update(m, t, v, m->music, 0);
    } else {
        unsigned a0;
        if (t[OSC] & 1) {
            t[OSC] &= 0x3f;
            t[27] = 0;
            t[26] = 0;
        }
        t[STAGE] = 0;
        a0 = tab(m, T_ATTACK, rb(m, t[35] | (unsigned)t[36] << 8));
        t[40] = (uint8_t)a0;
        t[41] = (uint8_t)(a0 >> 8);
        t[42] = (uint8_t)a0;
        t[43] = (uint8_t)(a0 >> 8);
        if (!m->sfx_on) {
            wr(m, v * 8u + 2u, 0);
            wr(m, 0x93, 0x27ff);
            wr(m, 0xd9, 0x07ff);
            wr(m, 0xbau + v, tab(m, T_VOLUME, t[ECHO]) << 2);
            wr(m, v * 8u + 1u, (unsigned)t[34] << 8 | t[33]);
            wr(m, v * 8u + 3u, 0x8000);
        }
        t[SILENT] = 0;
        voice_update(m, t, v, m->music, 0);
    }
    b = t[FLAGS];
    t[FLAGS] = (uint8_t)((b & 0x7fu) | ((b & 0x40u) << 1));
    if (t[FLAGS] & 0x80) {
        t[GATE_T] = 0;
        t[GATE_T + 1] = 0;
    } else {
        g = mul16((unsigned)t[DUR] | (unsigned)t[DUR + 1] << 8, (unsigned)t[GATE_RATIO] << 8);
        t[GATE_T] = (uint8_t)g;
        t[GATE_T + 1] = (uint8_t)(g >> 8);
    }
}

/* ---- control commands (0x0d62); returns 1 when the track ended ---- */
static int track_end(struct arcade_music *m, uint8_t *t, unsigned v) {
    if (m->music) {
        t[RUNNING] = 0;
        if (!m->sfx_on)
            key_off(m, v);
    } else {
        effect_end(m, v);
    }
    return 1;
}

static int control(struct arcade_music *m, unsigned c, uint8_t *t, unsigned v) {
    unsigned x, b, h, l, k;
    switch (c) {
    case 0: t[FLAGS] ^= 0x20; break;
    case 1: t[FLAGS] ^= 0x40; break;
    case 2: t[FLAGS] |= 0x10; break;
    case 3: t[FLAGS] ^= 0x08; break;
    case 4:
        b = rd(m, t);
        t[FLAGS] = (uint8_t)((t[FLAGS] & 0x97u) | b);
        break;
    case 5:
        h = rd(m, t);
        l = rd(m, t);
        if (m->music) {
            m->song_tempo = h << 8 | l;
        } else {
            t[51] = (uint8_t)h;
            t[50] = (uint8_t)l;
        }
        break;
    case 6: t[GATE_RATIO] = (uint8_t)rd(m, t); break;
    case 7:
        x = tab(m, T_VOLUME, rd(m, t));
        t[VOL] = (uint8_t)x;
        t[VOL + 1] = (uint8_t)(x >> 8);
        break;
    case 8: program(m, t, v, PROG_TABLE, rd(m, t)); break;
    case 9:
        b = rd(m, t);
        t[FLAGS] = (uint8_t)((t[FLAGS] & 0xf8u) | b);
        break;
    case 10:
        b = rd(m, t);
        if (m->music)
            m->transpose = b;
        break;
    case 11: t[TRANSPOSE] = (uint8_t)rd(m, t); break;
    case 12: t[FINE] = (uint8_t)rd(m, t); break;
    case 13: t[SLIDE] = (uint8_t)(rd(m, t) & 0x7fu); break;
    case 14: case 15: case 16: case 17:
        k = LOOPS + c - 14u;
        b = rd(m, t);
        if (t[k] == 0) {
            t[k] = (uint8_t)b;
            jump(m, t);
        } else {
            t[k] = (uint8_t)(t[k] - 1u);
            if (t[k])
                jump(m, t);
            else
                ptr_set(t, ptr_get(t) + 2u);
        }
        break;
    case 18: case 19: case 20: case 21:
        k = LOOPS + c - 18u;
        if (t[k] == 1) {
            t[k] = 0;
            b = rd(m, t);
            t[FLAGS] = (uint8_t)((t[FLAGS] & 0x97u) | b);
            jump(m, t);
        } else {
            ptr_set(t, ptr_get(t) + 3u);
        }
        break;
    case 22: jump(m, t); break;
    case 23: return track_end(m, t, v);
    case 24:
        b = rd(m, t);
        if (m->music) {
            t[PAN] = (uint8_t)b;
            if (!m->sfx_on)
                pan_write(m, v, b);
        }
        break;
    case 25: t[ECHO] = (uint8_t)rd(m, t); break;
    case 26:
        b = rd(m, t);
        if (m->music)
            m->song_vol = mul16(m->vol_scale, tab(m, T_LEVEL, b));
        break;
    case 27:
        x = tab(m, T_VIB_DEPTH, rd(m, t));
        t[28] = (uint8_t)x;
        t[29] = (uint8_t)(x >> 8);
        break;
    case 28:
        x = tab(m, T_TREM_DEPTH, rd(m, t));
        t[30] = (uint8_t)x;
        t[31] = (uint8_t)(x >> 8);
        break;
    case 29:
        x = tab(m, T_LFO_RATE, rd(m, t));
        t[24] = (uint8_t)x;
        t[25] = (uint8_t)(x >> 8);
        break;
    case 30:
        b = rd(m, t);
        t[OSC] = (uint8_t)((t[OSC] & ~1u) | b);
        break;
    case 31: program(m, t, v, PROG_TABLE2, rd(m, t)); break;
    default: break;
    }
    return 0;
}

/* ---- the track interpreter (0x0b07) ---- */
static void track_tick(struct arcade_music *m, uint8_t *t, unsigned v) {
    unsigned dur = t[DUR] | (unsigned)t[DUR + 1] << 8, de = m->tempo, count = 0;
    m->fault = 0;
    if (dur != 0) {
        unsigned gate = t[GATE_T] | (unsigned)t[GATE_T + 1] << 8;
        int r;
        if (gate) {
            r = (int)gate - (int)de;
            if (r > 0) {
                gate = (unsigned)r;
            } else {
                unsigned rt;
                t[STAGE] = 4;
                rt = tab(m, T_RATE, rb(m, (t[35] | (unsigned)t[36] << 8) + 4u));
                t[40] = (uint8_t)rt;
                t[41] = (uint8_t)(rt >> 8);
                gate = 0;
            }
            t[GATE_T] = (uint8_t)gate;
            t[GATE_T + 1] = (uint8_t)(gate >> 8);
        }
        r = (int)dur - (int)de;
        t[DUR] = (uint8_t)r;
        t[DUR + 1] = (uint8_t)((unsigned)r >> 8);
        if (r > 0 && (((unsigned)r >> 8) & 255u) < 0xc1u) {
            after_tick(m, t, v);
            return;
        }
    }
    for (;;) {
        unsigned a = rd(m, t);
        if (m->fault || ++count > MAX_COMMANDS) {   /* broken data: the Z80 would run on into whatever; the track is ended instead */
            m->fault = 0;
            track_end(m, t, v);
            return;
        }
        if (a < 0x20u) {
            if (control(m, a, t, v))
                return;
            continue;
        }
        note(m, a, t, v);
        return;
    }
}

/* ---- the fades of one pass (0x0175) ---- */
static void run_fades(struct arcade_music *m) {
    if (m->gfade_mode) {
        unsigned md = m->gfade_mode, lvl = m->gfade_level + m->gfade_rate, a = (0xffu + (md >> 7)) & 255u;
        a = (a + (lvl > 0xffffu ? 1u : 0u)) & 255u;
        lvl &= 0xffffu;
        if (a == 0) {
            m->gfade_mode = md & 0xfeu;
            lvl = 0;
        }
        m->gfade_level = lvl;
    }
    if (m->fade_mode) {
        unsigned tgt = m->fade_target, lvl = m->fade_level + m->fade_rate;
        int carry = lvl > 0xffffu;
        lvl &= 0xffffu;
        if (m->fade_mode == 1) {
            if (carry || (lvl >> 8) > tgt) {
                lvl = tgt << 8 | 0xffu;
                if (tgt == 0xffu)
                    m->fade_mode = 0;
            }
        } else if (!carry || (lvl >> 8) <= tgt) {
            lvl = tgt << 8;
        }
        m->fade_level = lvl;
    }
}

/* ---- one pass of the main loop (0x00f9) ---- */
static void run_pass(struct arcade_music *m) {
    unsigned v;
    for (v = 0; v < 16; v++) {
        uint8_t *s = m->sfx[v], *t = m->mus[v];
        m->music = 0;
        m->sfx_on = 0;
        if (s[RUNNING]) {
            if (s[63]) {
                s[63]--;
                if (s[63] == 0) {
                    key_off(m, 1);                 /* the Z80's A register still holds the old counter: voice 1, not v */
                    pan_write(m, v, s[PAN]);
                    m->sfx_on = 1;
                }
            } else {
                m->tempo = (unsigned)s[51] << 8 | s[50];
                track_tick(m, s, v);
                if (s[RUNNING])
                    m->sfx_on = 1;
            }
        }
        if (t[RUNNING]) {
            m->music = 1;
            m->tempo = (m->song_tempo + m->tempo_bend) & 0xffffu;
            track_tick(m, t, v);
            m->music = 0;
        }
    }
    run_fades(m);
}

void arcade_music_tick(struct arcade_music *m) {
    if (!m)
        return;
    if ((m->irq & FIRST_PASS_MASK) == 3u)
        run_pass(m);
    m->irq++;
    m->tick = m->irq;
}

/* ---- commands from the 68000 (0x01e6) ---- */
static void music_voice_released(struct arcade_music *m, unsigned v) {
    if (m->sfx[v][RUNNING] == 0) {
        key_off(m, v);
        pan_write(m, v, 0x10);
    }
}

static void start_song(struct arcade_music *m, unsigned lin) {
    unsigned v;
    m->song_tempo = 0;
    m->transpose = 0;
    m->fade_mode = 0;
    m->tempo_bend = 0;
    m->pitch_bend = 0;
    m->song_vol = mul16(m->vol_scale, 0xfffeu);
    for (v = 0; v < 16; v++) {
        unsigned off = m->d[lin + 1u + 2u * v] << 8 | m->d[lin + 2u + 2u * v];
        uint8_t *t = m->mus[v];
        if (off == 0) {
            int was = t[RUNNING];
            t[RUNNING] = 0;
            if (was)
                music_voice_released(m, v);
            continue;
        }
        memset(t, 0, 64);
        t[RUNNING] = 0xff;
        ptr_set(t, lin + off);
        t[49] = 0xff;
        t[OSC] = 1;
        t[STAGE] = 5;
        t[PAN] = 0x10;
        music_voice_released(m, v);
    }
}

static void effect_copy(struct arcade_music *m, const uint8_t *t, const uint8_t *iy, unsigned v) {
    unsigned w = (v + 1u) & 15u, b, rate, bound;
    uint8_t *u = m->sfx[w];
    if (m->eff_prio < u[PRIORITY])
        return;
    memcpy(u, t, 64);
    b = iy[7] < 0x20u ? iy[7] : 0x20u;
    rate = neg16((unsigned)iy[9] << 8 | iy[10]);
    bound = b > iy[8] ? b - iy[8] : 0u;
    u[58] = (uint8_t)bound;
    u[55] = (uint8_t)b;
    u[56] = 0;
    u[60] = (uint8_t)(rate >> 8);
    u[59] = (uint8_t)rate;
    u[63] = 4;
}

/* 0x02be: packet byte 6 (low 3 bits) picks the pan motion, bytes 7..12 are its parameters */
static void effect_pan_setup(struct arcade_music *m, uint8_t *t, const uint8_t *iy, unsigned v) {
    unsigned mo = iy[6] & 7u, p7 = iy[7], p8 = iy[8], rate = (unsigned)iy[9] << 8 | iy[10], pan, bound, q;
    t[PANMODE] = (uint8_t)(mo + 1u);
#define PUT(PAN_, BOUND_, RT_) \
    do { t[55] = (uint8_t)(PAN_); t[56] = 0; t[58] = (uint8_t)(BOUND_); t[60] = (uint8_t)((RT_) >> 8); t[59] = (uint8_t)(RT_); } while (0)
    if (mo == 0) {
        t[PANMODE] = 0;
        t[55] = (uint8_t)(p7 < 0x20u ? p7 : 0x20u);
    } else if (mo == 1) {
        pan = p7;
        if (pan >= 0x11u) {
            pan = pan < 0x20u ? pan : 0x20u;
            rate = neg16(rate);
        }
        t[55] = (uint8_t)pan;
        t[56] = 0;
        t[60] = (uint8_t)(rate >> 8);
        t[59] = (uint8_t)rate;
    } else if (mo == 2) {
        pan = p7;
        if (pan < 0x11u) {
            bound = pan + p8 < 0x20u ? pan + p8 : 0x20u;
        } else {
            pan = pan < 0x20u ? pan : 0x20u;
            rate = neg16(rate);
            bound = pan > p8 ? pan - p8 : 0u;
        }
        PUT(pan, bound, rate);
        t[57] = (uint8_t)p8;
    } else if (mo == 3 || mo == 5 || mo == 7) {
        bound = p7 + p8 < 0x20u ? p7 + p8 : 0x20u;
        PUT(p7, bound, rate);
        if (mo == 5) {
            t[62] = iy[11];
            t[61] = iy[12];
        }
        if (mo == 7)
            effect_copy(m, t, iy, v);
    } else {
        pan = p7 < 0x20u ? p7 : 0x20u;
        bound = pan > p8 ? pan - p8 : 0u;
        q = neg16(rate);
        PUT(pan, bound, q);
        if (mo == 6) {
            q = neg16((unsigned)iy[11] << 8 | iy[12]);
            t[62] = (uint8_t)(q >> 8);
            t[61] = (uint8_t)q;
        }
    }
#undef PUT
}

static void start_effect(struct arcade_music *m, unsigned lin, unsigned prio) {
    unsigned v;
    for (v = 0; v < 16; v++) {
        unsigned off = m->d[lin + 1u + 2u * v] << 8 | m->d[lin + 2u + 2u * v];
        uint8_t *t = m->sfx[v];
        if (off == 0)
            continue;
        if (prio < t[PRIORITY])
            continue;
        memset(t, 0, 64);
        t[RUNNING] = 0xff;
        ptr_set(t, lin + off);
        t[PRIORITY] = (uint8_t)prio;
        t[49] = 0xff;
        t[OSC] = 1;
        t[STAGE] = 5;
        m->eff_prio = prio;
        effect_pan_setup(m, t, m->iy, v);
        key_off(m, v);
        pan_write(m, v, t[PAN]);
    }
}

static void special(struct arcade_music *m) {
    const uint8_t *iy = m->iy;
    unsigned n = iy[1], v, rate;
    switch (n) {
    case 0:
        m->gfade_mode = 0;
        memset(m->sfx, 0, sizeof m->sfx);
        memset(m->mus, 0, sizeof m->mus);
        reset_chip_voices(m);
        break;
    case 1:
        for (v = 0; v < 16; v++)
            if (m->sfx[v][RUNNING])
                effect_end(m, v);
        break;
    case 2:
        v = iy[4] & 15u;
        if (m->sfx[v][RUNNING])
            effect_end(m, v);
        break;
    case 5:
        for (v = 0; v < 16; v++) {
            uint8_t *t = m->mus[v];
            if (t[RUNNING]) {
                t[RUNNING] = 0;
                if (m->sfx[v][RUNNING] == 0)
                    key_off(m, v);
            }
        }
        break;
    case 6:
    case 7:
        rate = (unsigned)iy[3] << 8 | iy[4];
        if (rate == 0)
            return;
        if (n == 6) {
            if (m->fade_mode == 0)
                m->fade_level = 0;
            m->fade_mode = 1;
            m->fade_rate = rate;
        } else {
            if (m->fade_mode == 0)
                m->fade_level = 0xffffu;
            m->fade_mode = 0xffu;
            m->fade_rate = neg16(rate);
        }
        m->fade_target = iy[2];
        break;
    case 8: {
        unsigned x = ((unsigned)iy[4] * 0x369u) >> 8;
        m->tempo_bend = iy[3] == 0xffu ? neg16(x) : x;
        break;
    }
    case 9: {
        unsigned x = (unsigned)iy[4] << 4;
        m->pitch_bend = iy[3] == 0xffu ? neg16(x) : x;
        break;
    }
    case 12:
    case 13:
        rate = (unsigned)iy[3] << 8 | iy[4];
        if (rate == 0)
            return;
        if (n == 12) {
            if (m->gfade_mode == 1)
                return;
            if (m->gfade_mode == 0)
                m->gfade_level = 0;
            m->gfade_mode = 1;
            m->gfade_rate = rate;
        } else {
            if (m->gfade_mode == 0xffu)
                return;
            if (m->gfade_mode == 0)
                m->gfade_level = 0xffffu;
            m->gfade_mode = 0xffu;
            m->gfade_rate = neg16(rate);
        }
        break;
    case 3:
        adpcm_reset(m);
        break;
    case 4:
        key_off(m, iy[4]);
        break;
    default:   /* 10, 11: the pan report to the 68000 (not modelled); 14: nothing */
        break;
    }
}

int arcade_music_packet(struct arcade_music *m, const uint8_t *packet, size_t size) {
    unsigned count, o, lin, code, h;
    if (!m || (!packet && size))
        return -1;
    memset(m->iy, 0, sizeof m->iy);
    if (size > sizeof m->iy)
        size = sizeof m->iy;
    if (size)
        memcpy(m->iy, packet, size);
    if (m->iy[0] == 0xffu) {
        special(m);
        return 0;
    }
    count = (unsigned)m->d[CMD_TABLE] << 8 | m->d[CMD_TABLE + 1];
    if (count == 0)
        return -1;
    code = (unsigned)m->iy[0] << 8 | m->iy[1];
    o = CMD_TABLE + 6u + 4u * (code % count);
    if (o + 3u >= ARCADE_SOUND_SIZE)
        return -1;
    lin = (unsigned)m->d[o] << 16 | (unsigned)m->d[o + 1] << 8 | m->d[o + 2];
    if (lin == 0 || lin + 33u > ARCADE_SOUND_SIZE)
        return -1;
    h = m->d[lin];
    if (h == 0) {
        start_song(m, lin);
        return 0;
    }
    if (h < 0x80u) {
        start_effect(m, lin, h);
        return 0;
    }
    return -1;
}

int arcade_music_command(struct arcade_music *m, unsigned code) {
    uint8_t p[16] = {0};
    p[0] = (uint8_t)(code >> 8);
    p[1] = (uint8_t)code;
    p[7] = 0x10;
    return arcade_music_packet(m, p, sizeof p);
}

void arcade_music_stop(struct arcade_music *m) {
    static const uint8_t p[2] = {0xff, 0};
    arcade_music_packet(m, p, sizeof p);
}

unsigned arcade_music_tracks(const struct arcade_music *m) {
    unsigned v, n = 0;
    if (!m)
        return 0;
    for (v = 0; v < 16; v++)
        n += (m->sfx[v][RUNNING] != 0) + (m->mus[v][RUNNING] != 0);
    return n;
}

/* ---- lifetime ---- */
void arcade_music_reset(struct arcade_music *m) {
    const uint8_t *d;
    const uint8_t *smp;
    int stereo;
    struct arcade_music_write *log;
    size_t cap;
    float gain;
    if (!m)
        return;
    gain = m->song_gain;
    d = m->d;
    smp = m->smp;
    stereo = m->stereo;
    log = m->log;
    cap = m->log_cap;
    memset(m, 0, sizeof *m);
    m->d = d;
    m->smp = smp;
    m->stereo = stereo;
    m->log = log;
    m->song_gain = gain;   /* the host's, kept */
    m->log_cap = cap;
    m->vol_scale = tab(m, T_LEVEL, m->d[CMD_TABLE + 2]);
    m->master_scale = tab(m, T_LEVEL, m->d[CMD_TABLE + 3]);
    reset_chip(m);
}

struct arcade_music *arcade_music_open(const struct arcade_rom *rom, int stereo) {
    struct arcade_music *m;
    if (!rom || !rom->sound)
        return NULL;
    m = (struct arcade_music *)calloc(1, sizeof *m);
    if (!m)
        return NULL;
    m->d = rom->sound;
    m->smp = rom->samples;
    m->stereo = stereo != 0;
    m->song_gain = 1.0f;   /* kept by the reset */
    arcade_music_reset(m);
    return m;
}

void arcade_music_close(struct arcade_music *m) {
    free(m);
}

void arcade_music_chip_write(struct arcade_music *m, unsigned reg, unsigned value) {
    if (m)
        wr(m, reg, value);
}

void arcade_music_set_log(struct arcade_music *m, struct arcade_music_write *buf, size_t cap) {
    if (!m)
        return;
    m->log = buf;
    m->log_cap = buf ? cap : 0;
    m->log_count = 0;
}

size_t arcade_music_log_count(const struct arcade_music *m) {
    return m ? m->log_count : 0;
}

void arcade_music_voice_state(const struct arcade_music *m, unsigned voice, struct arcade_music_voice *out) {
    const struct chip_voice *c;
    if (!m || !out || voice >= 16)
        return;
    c = &m->cv[voice];
    out->bank = c->bank;
    out->addr = (uint16_t)(c->pos >> 32);
    out->rate = c->rate;
    out->loop = c->loop;
    out->end = c->end;
    out->volume = c->volume;
    out->send = c->send;
    out->pan = c->pan;
}

/* ---- the mixer ---- */
static float clip16(float x) {
    return x > 32767.0f ? 32767.0f : x < -32768.0f ? -32768.0f : x;
}

static void mix_frame(struct arcade_music *m, float *left, float *right) {
    float l = 0.0f, r = 0.0f, el = 0.0f, er = 0.0f;
    unsigned i;
    for (i = 0; i < 16; i++) {
        struct chip_voice *c = &m->cv[i];
        uint64_t a = c->pos >> 32;
        if (c->end && a >= c->end) {
            if (c->loop) {
                uint64_t k = (a - c->end) / c->loop + 1u;
                c->pos -= (k * c->loop) << 32;
                a = c->pos >> 32;
            } else {
                c->pos = (uint64_t)c->end << 32;   /* no loop: the voice holds at the end and is silent */
                a = c->end;
            }
        }
        if (c->volume && m->smp && c->end && a < c->end) {
            const uint8_t *bank = m->smp + ((size_t)c->bank << 16);
            uint64_t nx = a + 1u;
            float s0, s1, f, s, gl, gr;
            if (nx >= c->end && c->loop)
                nx -= c->loop;
            s0 = (float)(int8_t)bank[a];
            s1 = (float)(int8_t)bank[nx & 0xffffu];
            f = (float)((c->pos & 0xffffffffu) >> 16) * (1.0f / 65536.0f);
            s = (s0 + (s1 - s0) * f) * (SAMPLE_SCALE * (float)c->volume * (1.0f / VOLUME_UNITY));
            if (!m->sfx[i][RUNNING])
                s *= m->song_gain;   /* the music's voice: the host's music volume */
            gl = pan_sin[32 - c->pan];
            gr = pan_sin[c->pan];
            l += s * gl;
            r += s * gr;
            if (c->send) {
                float e = (float)c->send * (1.0f / 32768.0f);
                el += s * gl * e;
                er += s * gr * e;
            }
        }
        if (c->end)
            c->pos += c->step;
    }
    if (m->echo_len > ECHO_LENGTH_BASE) {
        unsigned len = (unsigned)((uint64_t)(m->echo_len - ECHO_LENGTH_BASE) * ARCADE_MUSIC_RATE * CHIP_DIVIDER / CHIP_CLOCK_HALF);
        float fb = (float)m->echo_fb * (1.0f / 32768.0f), wl, wr_;
        if (len > ECHO_MAX)
            len = ECHO_MAX;
        if (fb > ECHO_FEEDBACK_MAX)
            fb = ECHO_FEEDBACK_MAX;
        if (len) {
            if (m->echo_pos >= len)
                m->echo_pos = 0;
            wl = m->echo_l[m->echo_pos];
            wr_ = m->echo_r[m->echo_pos];
            m->echo_l[m->echo_pos] = el + wl * fb;
            m->echo_r[m->echo_pos] = er + wr_ * fb;
            m->echo_pos++;
            l += wl;
            r += wr_;
        }
    }
    *left = clip16(l * OUTPUT_GAIN);
    *right = clip16(r * OUTPUT_GAIN);
}

void arcade_music_set_song_gain(struct arcade_music *m, float gain) {
    if (m)
        m->song_gain = gain < 0.0f ? 0.0f : gain > 1.0f ? 1.0f : gain;
}

int arcade_music_render(struct arcade_music *m, int16_t *out, size_t frames) {
    size_t i;
    if (!m || (!out && frames) || frames > 0x7fffffffu)
        return -1;
    for (i = 0; i < frames; i++) {
        float l, r;
        m->tick_acc += ARCADE_MUSIC_TICK_RATE;
        if (m->tick_acc >= ARCADE_MUSIC_RATE) {
            m->tick_acc -= ARCADE_MUSIC_RATE;
            arcade_music_tick(m);
        }
        mix_frame(m, &l, &r);
        out[2 * i] = (int16_t)l;
        out[2 * i + 1] = (int16_t)r;
    }
    return (int)frames;
}
