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
    void (*finish)(void);                    /* after the load, or null */
};

void port_loadwait(const struct port_loadwait_hooks *hooks, unsigned a, unsigned b);

#endif
