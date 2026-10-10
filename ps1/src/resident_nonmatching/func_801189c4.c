/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice, and the C is organised in a helper and loops where
 * the original jumps between labels (all read from the original's listing,
 * not tested). The exact owner of the bytes in the PS1 build stays the raw
 * bytes of the resident image; the build does not use this file. The
 * differential test next to it (difftest.py, with func_801189c4.py)
 * compares the calls this C makes and the memory it leaves with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not original names): the game's main. It never
 * returns and has no epilogue after its endless loop (read from the
 * original's listing, not tested). Start-up: an empty function
 * (func_80118900; inferred), two library calls (inferred ResetCallback and
 * StopCallback), a wait until the disc start-up function reports done, the
 * call that loads and runs the stand-alone program, a clear of seven
 * scratchpad words, the game_state set-up. Then the display is built
 * (func_80118d10 once per session, func_80118e58 / func_80119030 /
 * func_80119144 after each soft reset) and the frame loop runs. One frame:
 * reset root counter 1 (inferred ResetRCnt), read the pads unless
 * game_state.field_225 is set; if either pad check says reset, run the
 * soft-reset path (stop sound and disc, end three threads) and rebuild the
 * display. Otherwise, unless data_801ac620 is above 0x4000, run the game's
 * frame functions and count the frame in game_state.field_32. If the
 * interrupt-set scratchpad word 4 is not 0, end the threads and go back to
 * the session start with scratchpad words 0 and 8 set to -1. Otherwise read
 * root counter 1 into both halves of data_801903b0 (its second half again
 * after func_80157d9c(0) when data_801ac314 is not 0), wait until
 * data_801ac310 (counted up by an interrupt, inferred) reaches
 * data_801abef8 unless data_801abf0c is not 0 (each turn of the wait calls
 * func_80157d9c(1) when data_801ac314 is 0, and when that returns more than
 * 0 the second half of data_801903b0 is read again), call func_801578fc(1)
 * when data_801ac314 is 0, and run the two draw functions unless
 * data_801ac620 is above 0x4000. data_801ac620 then counts down, or gets
 * bit 4 set when its low five bits are 0.
 *
 * Contract (the roles named for the fields are inferred):
 *   Arguments: none. Result: none; the test ends the run at a call of the
 *   frame loop (see func_801189c4.py), and the stack pointer, the saved
 *   registers and everything main stores on its stack are not compared.
 *   Every callee is a recorder; none of them runs. The log shows the
 *   address, the arguments and the watched memory at each call. Recorded
 *   arguments are the ones main passes; the only pointer among them, the
 *   address of game_state passed to func_80155c90, is logged as an address
 *   (the whole of game_state is watched).
 *   Callees and what they return:
 *     func_8014f0bc  (0 args)  0 a random number of times, then 1
 *     func_80118fc8  (0 args)  random per call, 0 most often
 *     func_801576e8  (1 arg)   random per case
 *     func_80157d9c  (1 arg)   random per call, some above 0, some not
 *     every other callee returns 0 and its result is not used.
 *   Interrupts: the test stands in for them with recorders. The recorder of
 *     func_80157d9c (called in the wait loop and before it) adds 1 to the
 *     word at data_801ac310 at every call (the counter is the low halfword
 *     of that word; its neighbour at +2 has no reader in the resident
 *     image; inferred), and a recorder called once per frame
 *     stores 1 into scratchpad word 4 at a chosen frame, and in some cases
 *     0 again one frame later.
 *   Reads: game_state.field_225 and field_32, data_801ac620, data_801ac314,
 *     data_801abf0c, data_801abef8, scratchpad word 4, data_801ac310 (in
 *     the wait loop, after main cleared it).
 *   Writes: scratchpad words 0 to 0x18 (clear, and -1 at 0 and 8),
 *     game_state.field_1e, field_0a, field_0c, field_02, field_32,
 *     data_801ac310, data_801903b0 (two halfwords), data_801ac620.
 *   Watched at every call: game_state whole, scratchpad words 0 to 0x18,
 *     the word of data_801ac310, the word of data_801903b0, the word of
 *     data_801ac620.
 *   Aliasing: none of these overlap.
 *   Excluded: data_801ac314 not 0 with data_801abf0c 0 and data_801abef8
 *     not 0. Main calls nothing inside the wait loop then, so nothing in
 *     the test counts data_801ac310 up and the original does not leave the
 *     loop; the setup does not generate it.
 *   Not reached by any input (7 of 211 instruction slots): +0x330..+0x348,
 *     after the endless loop; nothing jumps there and the original has no
 *     epilogue (read from the original's listing, not tested).
 */
#include "../game.h"

#define ROOT_COUNTER_1 0xf2000001 /* inferred: the identifier of root counter 1 */

extern int scratch_word_00;
extern volatile int scratch_word_04; /* set by an interrupt (inferred) */
extern int scratch_word_08, scratch_word_0c, scratch_word_10, scratch_word_14, scratch_word_18;
extern volatile u16 data_801ac310; /* counted up by an interrupt (inferred) */
extern u16 data_801ac314;
extern u16 data_801ac620;
extern u16 data_801abef8;
extern u16 data_801abf0c;
extern u16 data_801903b0[];

void func_80118900(void);
void func_8015efc0(void);
void func_8015f0b4(void);
int func_8014f0bc(void);
void func_80150cd0(int a);
void func_80155c90(Object *o);
void func_80118d10(short a, short b);
void func_80118e58(int a, int b, int c, int d);
void func_80119030(void);
void func_80119144(int a, int b);
void func_80157784(unsigned counter);
int func_80118fc8(void);
void func_80120408(void);
void func_8014f4d4(int a, int b);
void func_80157d00(int a);
void func_80119340(int a);
void func_8011907c(void);
void func_8014f59c(void);
void func_80151a04(void);
int func_801576e8(unsigned counter);
int func_80157d9c(int a);
void func_801578fc(int a);
void func_801194f4(void);
void func_8011948c(void);

static void end_threads(void) {
    func_80157d00(0);
    func_80119340(0);
    func_80119340(1);
    func_80119340(2);
}

/* One run of frames. Returns 1 for a soft reset (the display is rebuilt), 0
   for a restart of the session. Does not return otherwise. */
static int run_frames(void) {
    for (;;) {
        int reset = 0;

        data_801ac310 = 0;
        func_80157784(ROOT_COUNTER_1);
        if (game_state.field_225 == 0) {
            reset = func_80118fc8();
            reset |= func_80118fc8();
        }
        if (reset != 0) {
            func_80120408();
            func_8014f4d4(4, 0);
            end_threads();
            return 1;
        }

        if (data_801ac620 <= 0x4000) {
            func_8011907c();
            func_8014f59c();
            func_80151a04();
            game_state.field_02 = 0;
            game_state.field_32 = game_state.field_32 + 1;
        }

        if (scratch_word_04 != 0) {
            end_threads();
            scratch_word_08 = -1;
            scratch_word_00 = -1;
            return 0;
        }

        data_801903b0[0] = data_801903b0[1] = func_801576e8(ROOT_COUNTER_1);
        if (data_801ac314 != 0) {
            func_80157d9c(0);
            data_801903b0[1] = func_801576e8(ROOT_COUNTER_1);
        }
        if (data_801abf0c == 0) {
            while (data_801ac310 < data_801abef8) {
                if (data_801ac314 == 0 && func_80157d9c(1) > 0) {
                    data_801903b0[1] = func_801576e8(ROOT_COUNTER_1);
                }
            }
        }
        if (data_801ac314 == 0) {
            func_801578fc(1);
        }

        if (data_801ac620 <= 0x4000) {
            func_801194f4();
            func_8011948c();
        }
        if ((data_801ac620 & 0x1f) != 0) {
            data_801ac620 = data_801ac620 - 1;
        } else {
            data_801ac620 = data_801ac620 | 0x10;
        }
    }
}

void func_801189c4(void) {
    func_80118900();
    func_8015efc0();
    func_8015f0b4();
    while (func_8014f0bc() == 0) {
    }
    func_80150cd0(0);

    scratch_word_14 = 0;
    scratch_word_10 = 0;
    scratch_word_0c = 0;
    scratch_word_08 = 0;
    scratch_word_04 = 0;
    scratch_word_00 = 0;
    scratch_word_18 = 0;
    game_state.field_1e = 0x1c3;
    func_80155c90((Object *)&game_state);
    game_state.field_0a = 0;
    game_state.field_0c = 0;

    for (;;) {
        func_80118d10(1, 1);
        do {
            func_80118e58(0x180, 0xf0, 0x180, 0xe0);
            func_80119030();
            func_80119144(0, 0);
        } while (run_frames());
    }
}
