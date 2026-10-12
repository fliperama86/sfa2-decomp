/* The experimental setting fast_loads (see loadwait.h): host routines that run in place of the game's two load-waiting routines,
 * installed only while the setting is on. Each starts the load as the game's C does and then, until the load is done, steps the
 * disc and the game's load step with no wait for frames. */
#include "loadwait.h"
#include "port.h"
#include "cd.h"
#include "guest.h"
#include "settings.h"

void port_loadwait(const struct port_loadwait_hooks *h, unsigned a, unsigned b)
{
    unsigned n;
    h->start(a, b);
    for (n = 1; !h->done(); n++) {
        h->step();   /* the frame's own order: the game's step, then the vblank (the disc's step) that ends the frame */
        h->drive();
        if (n % PORT_LOADWAIT_TICK_EVERY == 0) h->tick();
    }
    if (h->finish) h->finish();
}

/* The game's routines, by their addresses in the PS1's memory (the build's names are start_cd_track_from_table, is_cd_ctl_mode_3
 * and update_cd_control). */
#define GAME_FN(address) ((port_fn)(uintptr_t)(address))
#define FN_START_FROM_TABLE 0x8014f690u
#define FN_IS_MODE_3        0x8014f4bcu
#define FN_UPDATE_CD        0x8014f59cu
#define CTL                 0x80190948u   /* ctl_80190948, the load's control: the first argument of the start */
#define LOOP_MODE           0x8019032du   /* game_state.loop_mode (game_state is 0x80190108, loop_mode at 0x225) */
#define SCREEN_FLAGS        0x801ac620u   /* data_801ac620, a u16 */

static void start(unsigned a, unsigned b) { port_call3(GAME_FN(FN_START_FROM_TABLE), CTL, a, b); }
static int  done(void)                    { return port_callr0(GAME_FN(FN_IS_MODE_3)) != 0; }
static void drive(void)                   { port_cd_tick(); }
static void tick(void)                    { port_tick(); }

/* start_cd_track_wait_with_thread_switch: the waiting game thread gives the frame to the main thread, whose frame runs the step. */
static void step_plain(void)              { port_call0(GAME_FN(FN_UPDATE_CD)); }

/* start_cd_track_and_wait_updating_control: its own loop sets bit 0x8000 of the screen flags when the loop mode is 2, then steps. */
static void step_flagged(void)
{
    if (*(unsigned char *)port_gs((const void *)(uintptr_t)LOOP_MODE, 1) == 2)
        *(unsigned short *)port_gs((const void *)(uintptr_t)SCREEN_FLAGS, 2) |= 0x8000u;
    port_call0(GAME_FN(FN_UPDATE_CD));
}
static void finish_flagged(void)          { *(unsigned short *)port_gs((const void *)(uintptr_t)SCREEN_FLAGS, 2) &= 0x1fu; }

static const struct port_loadwait_hooks plain   = { start, done, step_plain,   drive, tick, 0 };
static const struct port_loadwait_hooks flagged = { start, done, step_flagged, drive, tick, finish_flagged };

static void port_o_wait_with_thread_switch(int a, int b);
static void port_o_wait_with_thread_switch(int a, int b) { port_loadwait(&plain, (unsigned)a, (unsigned)b); }
static void port_o_wait_updating_control(int a, int b);
static void port_o_wait_updating_control(int a, int b) { port_loadwait(&flagged, (unsigned)a, (unsigned)b); }

const struct port_override port_loadwait_overrides[] = {
    { "start_cd_track_wait_with_thread_switch", (void *)port_o_wait_with_thread_switch,
      "starts the load and steps the disc and the game's load step with no wait for frames until it is done (setting fast_loads)", &port_settings.fast_loads },
    { "start_cd_track_and_wait_updating_control", (void *)port_o_wait_updating_control,
      "the same, with that routine's own screen flag set before each step and cleared after (setting fast_loads)", &port_settings.fast_loads },
    { NULL, NULL, NULL, NULL }
};
