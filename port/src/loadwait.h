/* The experimental setting fast_loads: the game's two routines that wait for a load, run without waiting for frames.
 *
 * The game loads its data (sound banks, fonts, the demo) through a state machine that gets one step per frame
 * (update_cd_control) and a disc that gives sectors at the console's drive rate, counted by the frame. The routines
 * start_cd_track_wait_with_thread_switch and start_cd_track_and_wait_updating_control start a load and wait, a frame at a time
 * (a switch to the main thread, whose frame does the step, or a VSync), until the load's control says it is done. The frame is
 * only the clock of that wait: the port's override does the same two things in the frame's own order, the game's own step
 * (update_cd_control) and then a drive step (port_cd_tick, the vblank's: the commands' second responses, the sectors of one
 * vblank, the ring guard), with no wait for the host's clock, so the game sees the same sequence of frames it saw before, in less time.
 *
 * Not run in the wait: the game's own vblank handlers, the main thread's frame (drawing, pads, the voice threads) and the
 * wait for the display. Every PORT_LOADWAIT_TICK_EVERY steps one real port_tick runs, so that the window and the host's own
 * work (audio, events, a vblank that is due) go on.
 * The loop is a function of hooks, so that a test can run it with made-up routines. */
#ifndef PORT_LOADWAIT_H
#define PORT_LOADWAIT_H

#define PORT_LOADWAIT_TICK_EVERY 256u

struct port_loadwait_hooks {
    void (*start)(unsigned a, unsigned b);   /* start_cd_track_from_table(&ctl, a, b) */
    int  (*done)(void);                      /* is_cd_ctl_mode_3() */
    void (*step)(void);                      /* the game's step: (the wait's own flag,) update_cd_control() */
    void (*drive)(void);                     /* port_cd_tick(): one vblank of the disc */
    void (*tick)(void);                      /* port_tick(): the host's real clock and events */
    void (*hurry)(int on);                   /* port_clock_hurry: 1 before the load starts, 0 after it is done (the clock does not sleep inside) */
    void (*finish)(void);                    /* after the load, or null */
};

void port_loadwait(const struct port_loadwait_hooks *hooks, unsigned a, unsigned b);

/* The loading screen. The game opens a thread of its own for the time of a load (its entry is loop_state_table_then_close_sound_thread):
 * a script of five states, one a frame. State 0 starts it, state 1 fades the palette in over 32 frames, state 2 holds, with the text "NOW
 * LOADING...", until the load's caller clears the loop mode, state 3 fades out over 32 frames, state 4 clears the wait flag that ends the
 * thread (which then restores the palette). It is counted in frames, so a faster load does not shorten it. With the setting on, the
 * routines of states 0 and 3 do in one call what the fade of their states ends with, so the script runs in four frames (or until the
 * load is done) and draws no text:
 *  - state 0 ends where the fade-in ends: the state index two on, the delay 1, the counter 0x20, the wait flag 0x80, the palette at
 *    full scale once (the fade-in's last step), and the text's string pointer on an empty string;
 *  - state 3 ends where the fade-out ends: the state index one on, the counter -1, the delay 1; no palette step (the thread's end
 *    restores the palette in any case). It also starts the blank fast-forward of the clock (kernel.c): the frames that follow, until the next
 *    screen is drawn, are not waited for. */
/* The game's black fades (fade_palette_bank, one step a call). Every fade of a whole screen to or from black is a thread that calls the
 * palette routine once a frame until the routine says it is done: mode 0 darkens one notch a call, mode 1 brightens to the backup
 * palette, a level that moves by `amount`. With the setting on, a call in mode 0 or 1 runs the routine's own steps one after the
 * other until it says done (at most PORT_FADE_MAX_STEPS), so the fade is over in one frame with the palette where the last frame of
 * the fade would have left it. Modes 2 and 3 (the white flashes of the fights, which count their frames themselves) are one step a call as before. */
#define PORT_FADE_MAX_STEPS 256u
unsigned port_fade_to_end(unsigned (*step)(void *ctx, unsigned bank, int amount, unsigned mode), void *ctx, unsigned bank, int amount, unsigned mode);

struct port_loadscreen_fields {
    unsigned short *state_index, *step_counter, *step_delay;   /* the game's own fields, as host pointers */
    unsigned char *wait_flag;
    unsigned *text_ptr;                                        /* the text item's string pointer (a guest address) */
    unsigned empty_string;                                     /* the guest address of the text's terminator "@@", where an empty string would stand */
    void (*palette_full)(void);                                /* scale_palette_rows_from_backup_bank(4, 0x20, 0x1f, 0) and queue_image_upload_job(4, 7) */
    void (*blank_start)(void);                                 /* port_clock_blank_start: the frames after the loads, until the next screen, run without the wait */
};
void port_loadscreen_start(const struct port_loadscreen_fields *f);
void port_loadscreen_fadeout(const struct port_loadscreen_fields *f);

#endif
