/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

#define ROOT_COUNTER_1 0xf2000001 /* inferred: the identifier of root counter 1 */

/* scratch_word_00, scratch_word_04 (set by an interrupt; inferred) and scratch_word_10 are declared in
   externs.h, as are data_801ac314, data_801ac620 and data_801abef8. */
extern int scratch_word_08, scratch_word_0c, scratch_word_14, scratch_word_18;
extern volatile u16 data_801ac310; /* counted up by an interrupt (inferred) */
extern u16 data_801abf0c;
extern u16 data_801903b0[];
extern u8 data_801a6964[]; /* the first pad's buffer (inferred) */
extern u8 data_801a6970[]; /* the second pad's buffer (inferred) */

void noop_empty_function(void);
void init_compare_limit_and_copy_bytes(Object *o);
void init_system_and_pads(short a, short b);
void init_display_buffers(int a, int b, int c, int d);
void init_snd_voices(void);
void func_80157784(unsigned counter);
int count_pad_records_with_all_buttons_and_flag(u8 *pad, int number);
void start_pending_snd_voice_threads(void);
void draw_text_items(void);
int func_801576e8(unsigned counter);
void swap_buffers_and_submit_draw_list(void);
void update_voice_delay_timers(void);

/*
 * What it does (inferred, not original names): the game's main. It never
 * returns: its epilogue, after the endless loop, is never reached. Start-up:
 * an empty function (noop_empty_function; inferred), two library calls (inferred
 * ResetCallback and StopCallback), a wait until the disc start-up function
 * reports done, the call that loads and runs the stand-alone program, a
 * clear of seven scratchpad words, the game_state set-up. Then the display
 * is built (init_system_and_pads once per session, init_display_buffers / init_snd_voices /
 * set_voice_thread_entry_and_open after each soft reset) and the frame loop runs. One frame:
 * reset root counter 1 (inferred ResetRCnt), read the pads unless
 * game_state.loop_mode is set; if either pad check says reset, run the
 * soft-reset path (stop sound and disc, end three threads) and rebuild the
 * display. Otherwise, unless data_801ac620 is above 0x4000, run the game's
 * frame functions and count the frame in game_state.tick_count. If the
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
 */
void init_system_and_loop_frames_forever(void) {
    int reset;

    noop_empty_function();
    func_8015efc0();
    func_8015f0b4();
    while (init_cd_menu() == 0) {
    }
    load_and_exec_program_by_index(0);
    scratch_word_14 = 0;
    scratch_word_10 = 0;
    scratch_word_0c = 0;
    scratch_word_08 = 0;
    scratch_word_04 = 0;
    scratch_word_00 = 0;
    scratch_word_18 = 0;
    game_state.field_1e = 0x1c3;
    init_compare_limit_and_copy_bytes((Object *)&game_state);
    game_state.field_0a = 0;
    game_state.field_0c = 0;
session:
    init_system_and_pads(1, 1);
display:
    init_display_buffers(0x180, 0xf0, 0x180, 0xe0);
    init_snd_voices();
    set_voice_thread_entry_and_open(0, 0);
    for (;;) {
        /* The counter is volatile (an interrupt counts it up; inferred): declared without it, this function differs from the original in 5 instruction slots. */
        data_801ac310 = 0;
        reset = 0;
        func_80157784(ROOT_COUNTER_1);
        if (game_state.loop_mode == 0) {
            /* Each call passes a pad's buffer and its number; the callee uses neither. Written without the arguments, this function is 20 bytes shorter than the original. */
            reset = count_pad_records_with_all_buttons_and_flag(data_801a6964, 0);
            reset |= count_pad_records_with_all_buttons_and_flag(data_801a6970, 1);
        }
        if (reset != 0) {
            stop_all_keys_clear_table();
            set_menu_command_by_code(4, 0);
            func_80157d00(0);
            close_voice_thread(0);
            close_voice_thread(1);
            close_voice_thread(2);
            goto display;
        }
        if (data_801ac620 <= 0x4000) {
            start_pending_snd_voice_threads();
            update_cd_control();
            draw_text_items();
            game_state.pass_flag = 0;
            game_state.tick_count++;
        }
        if (scratch_word_04 != 0) {
            func_80157d00(0);
            close_voice_thread(0);
            close_voice_thread(1);
            close_voice_thread(2);
            scratch_word_08 = -1;
            scratch_word_00 = -1;
            goto session;
        }
        /* One statement for both halves: as two statements this function differs from the original in 4 instruction slots. */
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
            swap_buffers_and_submit_draw_list();
            update_voice_delay_timers();
        }
        if ((data_801ac620 & 0x1f) != 0) {
            data_801ac620--;
        } else {
            data_801ac620 |= 0x10;
        }
    }
}
