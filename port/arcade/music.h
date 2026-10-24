/* The arcade's music and sound-effect player: the driver of the Z80 sound program as plain C of our own, and a mixer for the QSound chip's
 * sixteen voices. No Z80 code is run or interpreted and nothing is decrypted: the player reads only data of the sound program (the song
 * and effect tables, the instrument and envelope records and the driver's curve tables) and the QSound sample ROM.
 *
 * music.md is the specification and music.py its executable reference. The driver here reproduces music.py's register writes (tick,
 * register, value) exactly; `arcade_music_set_log` exposes them so that the tests compare the two. The mixer is the chip: it reads the
 * register writes, so the one place where driver and sound meet is the chip's register file.
 *
 * Time. The driver gets 250 interrupts a second (`arcade_music_tick`); every fourth is a pass that runs the tracks. The mixer turns the
 * chip into 44,100 Hz stereo, 16 bits, and calls the driver at the right frame itself (`arcade_music_render`).
 *
 * Mixer choices that music.md does not give (guesses, in music.c as named constants): the output is mixed in float; a voice reads
 * the 8-bit sample at `rate / 4096` chip samples per chip output sample (24,038 Hz = 60 MHz / 2 / 1248), the position is kept with 32
 * fraction bits, and is wrapped by the loop length at the end address (`position >= end`); linear interpolation; register 3 (phase) is the
 * fraction of a sample, 16 bits; volume 0x2000 is unity; the pan is equal power (cos, sin of p * pi / 64) in place of the chip's table;
 * the echo is a stereo delay line fed by each voice's panned output times its send (register / 0x8000), with the feedback
 * (register 0x93 / 0x8000) and a length in the chip's samples of (register 0xd9 - 0x554); output gain a constant.
 *
 * Project-authored, plain C99. No byte of either game is here. */
#ifndef ARCADE_MUSIC_H
#define ARCADE_MUSIC_H

#include <stddef.h>
#include <stdint.h>
#include "rom.h"

#define ARCADE_MUSIC_RATE 44100u        /* the mixer's output rate, frames a second */
#define ARCADE_MUSIC_TICK_RATE 250u     /* the driver's interrupts a second */
#define ARCADE_MUSIC_VOICES 16u

struct arcade_music;

/* One write to the chip, as music.py's `Driver.writes` lists them: the interrupt count when it happened, the register, the 16-bit value. */
struct arcade_music_write {
    uint32_t tick;
    uint16_t reg;
    uint16_t value;
};

/* A player over the views of `rom`: the Z80 program (rom->sound, ARCADE_SOUND_SIZE bytes) and the samples (rom->samples,
 * ARCADE_SAMPLES_SIZE bytes; may be NULL: the player then runs but is silent). The pointers are borrowed, not copied: they must outlive the
 * player. `stereo` 0 forces every pan to the centre, as the arcade's mono setting does. NULL when rom or rom->sound is NULL, or on no memory. */
struct arcade_music *arcade_music_open(const struct arcade_rom *rom, int stereo);
void arcade_music_close(struct arcade_music *m);

/* Power-on state: every track and fade cleared, the chip set up as the driver does (the log, if set, gets those writes), the mixer silent. */
void arcade_music_reset(struct arcade_music *m);

/* Play a song or a sound effect by its command code, an effect with a centred fixed pan. Returns 0 when something started, -1 when the
 * code is empty, an ADPCM sound (not played) or a bad argument. A song replaces the previous song; an effect takes its voices unless an
 * effect of higher priority holds them. */
int arcade_music_command(struct arcade_music *m, unsigned code);

/* The packet the 68000 hands the Z80, up to 16 bytes (music.md section 2): the code high byte first, then the parameters (byte 6 low three
 * bits the pan motion, bytes 7 to 12 its parameters); byte 0 = 0xff is a special command whose number is byte 1 (section 9). Missing bytes
 * are zero. Returns as arcade_music_command; special commands return 0. */
int arcade_music_packet(struct arcade_music *m, const uint8_t *packet, size_t size);

/* Stop everything (special command 0), at once. */
void arcade_music_stop(struct arcade_music *m);

/* One 250 Hz interrupt of the driver (what render does by itself at the right frames). */
void arcade_music_tick(struct arcade_music *m);

/* Write a register of the chip, as the driver does; for tests that play the mixer by hand (music.md section 7 lists the registers). */
void arcade_music_chip_write(struct arcade_music *m, unsigned reg, unsigned value);

/* Make `frames` frames of 44,100 Hz stereo, 16-bit, interleaved left then right, into out (2 * frames values), running the driver. Returns the
 * number of frames made (= frames), or -1 on a bad argument. */
int arcade_music_render(struct arcade_music *m, int16_t *out, size_t frames);
/* The host's volume of the music (0 to 1, 1 at the start): the mixer multiplies by it the voices that no effect is running on, so a
 * game's music volume and its fades reach the songs and not the effects. It changes no register the driver writes. */
void arcade_music_set_song_gain(struct arcade_music *m, float gain);

/* Number of tracks running (songs' and effects'), for the caller to know when a sound has ended. */
unsigned arcade_music_tracks(const struct arcade_music *m);

/* Record the chip writes into buf[0..cap): later writes are counted but not kept once it is full. NULL stops the record; the count restarts. */
void arcade_music_set_log(struct arcade_music *m, struct arcade_music_write *buf, size_t cap);
/* Number of writes since the log was set (may be more than cap). */
size_t arcade_music_log_count(const struct arcade_music *m);

/* What the mixer holds of a voice, for the tests of the mixer. */
struct arcade_music_voice {
    uint8_t bank;
    uint16_t addr, rate, loop, end, volume, send;
    uint8_t pan;     /* 0 left .. 32 right */
};
void arcade_music_voice_state(const struct arcade_music *m, unsigned voice, struct arcade_music_voice *out);

#endif
