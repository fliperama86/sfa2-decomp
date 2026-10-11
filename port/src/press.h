/* Sony's movie decoder library, as the PC program has it: the five calls of the library that the logo's program uses
 * (DecDCTReset, DecDCTvlc, DecDCTin, DecDCTout, DecDCToutCallback), on top of the decoder chip's core that the picture library
 * (PsyZ) has since the port's pin of 2026-10-10 (Psyz_MdecReset, Psyz_MdecCommand, Psyz_MdecRead), with the bitstream reader and
 * the reset tables of the port's own mdec.c (port_mdec_vlc, port_mdec_reset). The port's own decode routine in mdec.c is NOT used
 * here. The port's list of programs (port/programs.toml) binds the calls for the logo's program. Written from what the logo program's own copy of the library
 * does (read in a private listing) and from mdec.h; nothing here is compared with a console's library or its output. The chip's
 * pixels are the library's (tested upstream against captures from a console); what this file adds is tested in
 * port/tools/test_hostpress.py against direct calls of the same core.
 *
 * Every pointer is a game address, which is a real address in the PC program. Every read and write of the game's memory is checked
 * to lie inside the mapped RAM (PORT_RAM_BASE to PORT_RAM_BASE + PORT_RAM_SIZE); a range outside it, and every case below that is
 * not modelled, ends the program with a `stop:` line that names the call (port_halt, status PORT_EXIT_OTHER), in the way cd.c stops
 * on a command it does not do.
 *
 * THE COMPLETION OF AN OUTPUT TRANSFER. DecDCTout makes a completion pending, and the routine registered for DMA channel 1
 * (port_h_DMACallback(1, f), or DecDCToutCallback) is called when it is taken, in the pattern of the sound chip's transfer in
 * iopage.c: states none, pending, claimed by the timer thread, taken; taken exactly once, by the poll of the game's thread
 * (port_press_poll, from port_tick) or by the timer thread's claim (port_press_claim_due) and the interruption routine's delivery
 * (port_press_deliver_claimed); the state is cleared before the routine is called, because the routine starts the next transfer;
 * port_handler_depth is raised around the routine. With no routine registered the completion is dropped and counted.
 *
 * SEVERAL IN ONE CALL. When the routine has started the next transfer and that one's due time has passed too, its completion is
 * taken in the same call, by the poll or by the delivery alike, and so on until a transfer's due time is still ahead or the
 * routine starts none; port_handler_depth stays raised by one for all of them. (A completion that the timer has claimed is not
 * taken this way: it is left to the delivery.) On the console the interrupt of a transfer that is
 * done is taken as soon as the routine before returns, and nothing of the interrupted code runs in between (inferred from how the
 * console takes a pending interrupt; not measured on one). Here the poll comes after a sleep of up to 2 ms (port_tick) and the
 * timer's round waits 1 ms (interrupt.c), while the transfers of a frame of the logo's are due 0.8 ms apart: with one completion
 * a round the 20 transfers of a frame were not done within the frame's time, and the logo's program reset the decoder with
 * transfers of the frame still to come (the port's page has the counts of a run). A completion is still never taken before its
 * due time. Whatever the routine did is read again before each further one: the state, the due time, the clock and the routine
 * that is registered.
 *
 * THE DUE TIME is THIS PROJECT'S choice and not the chip's known pace. A completion must not come sooner than a chip could have
 * made the data (the logo's program relies on the routine not running inside DecDCTout). The completion of a transfer is due at
 * (the time of the frame's DecDCTin) + (the number of macroblocks of the frame handed out up to and including this transfer, rounded
 * up; a macroblock is 768 bytes at 24 bits and 512 at 15) * PORT_PRESS_US_PER_MACROBLOCK. 55 microseconds a macroblock makes a frame
 * of 320 by 240 (300 macroblocks) done about 16.5 ms after its input, half the period of a movie of 30 frames a second. The due times
 * hang on the frame's input and not on when the transfer before was taken, so the timer's lateness does not add up.
 *
 * DIFFERENCE FROM THE SOUND TRANSFER'S POLL: iopage.c's poll takes a pending completion at once, with no due time. port_press_poll
 * takes it only when its due time has passed; before that nothing happens.
 */
#ifndef PORT_PRESS_H
#define PORT_PRESS_H

#define PORT_PRESS_US_PER_MACROBLOCK 55u

/* DecDCTReset(mode). 0: ResetCallback (the runtime's), then the chip is reset and sent the two tables that Sony's library sends at
 * its reset, as the chip's commands: 40000001h with the 128 bytes of the two quantisation tables, then 60000000h with the 64
 * halfwords (little-endian) of the scale table, the values being those of port_mdec_reset; a pending output completion is dropped
 * with no call. 1: the chip's reset and the dropped completion, without ResetCallback and without the tables. Another mode: the
 * console's library prints a line and ends the program; here the program stops with a line. (Before the first DecDCTReset(0) the
 * chip's tables are whatever the library starts with; the logo's program resets first.) */
void port_DecDCTReset(int mode);

/* DecDCTvlc(bs, buf): `bs` is one frame's bitstream with its 8-byte header. Writes at `buf` the first two halfwords of `bs`
 * unchanged (the size in words and 3800h: together the chip's command word), then the run-level codes exactly as port_mdec_vlc
 * gives them, then 64 halfwords of FE00h, and nothing after that. Returns 0. A version that is not 1, 2 or 3: the two header
 * halfwords are written, nothing else, and the result is -1. A null `bs` (the library's continuation of a decode in parts): the
 * program stops. Any other failure of port_mdec_vlc: the program stops with the call's name and the reader's result (the console's
 * library would go on with what it decoded; not modelled). The reader is given the end of the mapped RAM as the limit of both
 * pointers; the codes and the 64 halfwords must fit before it.
 *
 * THE TIME THE DECODE TAKES. On the console an output transfer that was under way when this call was made is done, with its
 * routine run, before the call returns: INFERRED from the sizes (a frame of 300 macroblocks is 1,800 blocks, each with a DC value
 * and an end code at least, so 3,600 codes or more to read in software, against one transfer of 15 macroblocks by the chip); not
 * measured on a console. Both movie programs rely on it: each starts a frame's first transfer, calls DecDCTvlc for the next
 * frame, and then waits while a flag is not 0 that the transfers' routine sets to 1 when it starts a further transfer and to 0
 * after the last. On a PC the decode takes microseconds and the flag is still 0 from the frame before. So when the call has written its output and a completion is pending or claimed,
 * it ticks (port_tick) until one completion has been taken (the pending one at its due time, never before; with the ones that are
 * taken in the same call) or none is pending any more, and returns then; it does not wait for the frame's other transfers. It
 * does not wait: with nothing pending; where no interruption is allowed (inside a handler, interrupts off: the console's call
 * is not interrupted there either); when the result is -1. If no completion comes in a hundred thousand ticks (the clock does
 * not move) the program stops. No time is modelled beyond this. */
int port_DecDCTvlc(unsigned long *bs, unsigned long *buf);

/* DecDCTin(buf, mode): first changes the word at `buf` and writes it back: bit 27 cleared when `mode` has bit 0, set otherwise;
 * bit 25 set when `mode` has bit 1, cleared otherwise; no other bit. The word is then the chip's decode command: bits 31 to 29
 * must be 1, bits 28 and 27 give the depth (2: 24 bits, 3: 15 bits; 0 and 1 are the monochrome depths, no program here uses
 * them), bit 26 signed, bit 25 the mask bit. The parameters are the words after it, as many as its low 16 bits say; the command
 * and they are handed to the chip, and the time of this call and the depth are kept. (Sony's routine sends whole blocks of 32
 * words; a count that is not a multiple of 32 is not modelled.) The program stops when: the command is not a decode; the depth is
 * 0 or 1; the count is not a multiple of 32; the words are not inside the RAM; an output transfer is pending; the chip refuses. */
void port_DecDCTin(unsigned long *buf, int mode);

/* DecDCTout(buf, words): reads the next 32 * (words >> 5) words of the decoded frame from the chip into `buf` (whole blocks of 32
 * words) and makes the transfer's completion pending. If the completion of the transfer before is still pending, the call waits
 * for it first, as Sony's routine does: it ticks (port_tick), and the tick's poll takes the completion when its due time has come,
 * never before; if the routine starts a further transfer the wait goes on for that one too. Where no interruption is allowed
 * (inside a handler, interrupts off) the wait could never end and the program stops, as it does when the clock does not move for
 * a hundred thousand ticks. When the chip's read fails (more
 * is asked than the frame holds, or the codes are broken) the program stops (on the console the transfer would wait for more
 * input; not modelled). */
void port_DecDCTout(unsigned long *buf, int words);

/* DecDCToutCallback(f): the library's is DMACallback(1, f); this one calls port_h_DMACallback(1, f) and returns its result. */
void (*port_DecDCToutCallback(void (*f)(void)))(void);

/* The completion, called from kernel.c next to the sound transfer's calls: port_press_poll on the game's thread from port_tick,
 * under the same condition as the sound transfer's poll; port_press_claim_due by the timer thread (1: claimed, due time passed);
 * port_press_deliver_claimed by the interruption routine, on the game's thread. */
void port_press_poll(void);
int  port_press_claim_due(void);
void port_press_deliver_claimed(void);

/* Not 0 while an output transfer's completion is pending (DecDCTout made it, nothing has taken it), claimed by the timer thread, or
 * taken and about to run; 0 otherwise, and 0 while the completion's routine runs (the state is cleared before the call). The movie's
 * stream calls (stream.c) ask it for their busy rule. */
int port_press_out_busy(void);

/* One line on the output, then the four counts begin again at 0:
 *     decoder: program NAME: F frames, T transfers, C completions taken, R dropped by a reset
 * F: calls of DecDCTin that handed the chip a frame. T: transfers that DecDCTout started. C: completions taken (the routine was called,
 * or there was none and the completion was dropped). R: completions that a DecDCTReset found pending or claimed and dropped; the
 * frame they belong to was not handed out to its end. T - C - R is what was pending when the line was printed. programs.c calls it
 * when a built-in program has returned, with the program's name. */
void port_press_report(const char *program);

#endif
