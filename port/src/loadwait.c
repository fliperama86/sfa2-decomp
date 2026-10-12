/* The experimental setting fast_loads (see loadwait.h): host routines that run in place of the game's two load-waiting routines,
 * installed only while the setting is on. Each starts the load as the game's C does and then, until the load is done, steps the
 * disc and the game's load step with no wait for frames. */
#include "loadwait.h"
#include "port.h"
#include "cd.h"
#include "guest.h"
#include "settings.h"
#include "port_tables.h"
#include <string.h>

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

unsigned port_fade_to_end(unsigned (*step)(void *ctx, unsigned bank, int amount, unsigned mode), void *ctx, unsigned bank, int amount, unsigned mode)
{
    unsigned n, done = step(ctx, bank, amount, mode);
    if (mode > 1) return done;
    for (n = 1; !done && n < PORT_FADE_MAX_STEPS; n++) done = step(ctx, bank, amount, mode);
    return done;
}

void port_loadscreen_start(const struct port_loadscreen_fields *f)
{
    *f->state_index = (unsigned short)(*f->state_index + 2);
    *f->step_delay = 1;
    *f->step_counter = 0x20;
    *f->wait_flag = 0x80;
    f->palette_full();
    *f->text_ptr = f->empty_string;
}

void port_loadscreen_fadeout(const struct port_loadscreen_fields *f)
{
    *f->state_index = (unsigned short)(*f->state_index + 1);
    *f->step_delay = 1;
    *f->step_counter = 0xffffu;
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

/* The loading screen's fields (see loadwait.h): game_state is 0x80190108 (step_counter at 0xc2, step_delay_timer at 0xc4, wait_flag at 0xf0);
 * data_8018f5a0 holds the pointer to the structure whose halfword at 0x48 is the state index; the text item is at 0x8017f2ec with its
 * string pointer at 0x8017f2f8; the string "NOW LOADING...@@" is at 0x8016da10 (the game ends its text with "@@", not a NUL) and its terminator at 0x8016da1e. */
#define HUD_POINTER         0x8018f5a0u
#define STEP_COUNTER        0x801901cau
#define STEP_DELAY          0x801901ccu
#define WAIT_FLAG           0x801901f8u
#define LOADING_TEXT_PTR    0x8017f2f8u
#define EMPTY_STRING        0x8016da1eu
#define FN_SCALE_PALETTE    0x80125dc0u
#define FN_QUEUE_UPLOAD     0x80137220u

static void palette_full(void)
{
    port_call4(GAME_FN(FN_SCALE_PALETTE), 4, 0x20, 0x1f, 0);
    port_call2(GAME_FN(FN_QUEUE_UPLOAD), 4, 7);
}

static void screen_fields(struct port_loadscreen_fields *f)
{
    unsigned hud = *(unsigned *)port_gs((const void *)(uintptr_t)HUD_POINTER, 4);
    f->state_index = port_gs((const void *)(uintptr_t)(hud + 0x48u), 2);
    f->step_counter = port_gs((const void *)(uintptr_t)STEP_COUNTER, 2);
    f->step_delay = port_gs((const void *)(uintptr_t)STEP_DELAY, 2);
    f->wait_flag = port_gs((const void *)(uintptr_t)WAIT_FLAG, 1);
    f->text_ptr = port_gs((const void *)(uintptr_t)LOADING_TEXT_PTR, 4);
    f->empty_string = EMPTY_STRING;
    f->palette_full = palette_full;
}

static const struct port_loadwait_hooks plain   = { start, done, step_plain,   drive, tick, 0 };
static const struct port_loadwait_hooks flagged = { start, done, step_flagged, drive, tick, finish_flagged };

static void port_o_wait_with_thread_switch(int a, int b);
static void port_o_wait_with_thread_switch(int a, int b) { port_loadwait(&plain, (unsigned)a, (unsigned)b); }
static void port_o_wait_updating_control(int a, int b);
static void port_o_wait_updating_control(int a, int b) { port_loadwait(&flagged, (unsigned)a, (unsigned)b); }

/* fade_palette_bank(u8 bank, s16 amount, u8 mode): the game's own routine (its implementation in this program, found by name once) is
 * the step. The caller's argument words are narrowed to the types the routine has. */
static unsigned fade_step(void *ctx, unsigned bank, int amount, unsigned mode)
{
    return port_callr3((port_fn)(uintptr_t)ctx, bank, (unsigned)amount, mode) & 0xffu;
}
static unsigned port_o_fade_palette_bank(unsigned bank, int amount, unsigned mode);
static unsigned port_o_fade_palette_bank(unsigned bank, int amount, unsigned mode)
{
    static void *impl;
    unsigned i;
    if (!impl)
        for (i = 0; i < port_function_count; i++)
            if (port_functions[i].image == -1 && !strcmp(port_functions[i].name, "fade_palette_bank")) impl = port_functions[i].impl;
    if (!impl) port_halt(PORT_EXIT_OTHER, "fast_loads: fade_palette_bank has no implementation in this build");
    return port_fade_to_end(fade_step, impl, bank & 0xffu, (short)amount, mode & 0xffu);
}

/* The game's state routines take the game state's address; the override does not read it. */
static void port_o_loadscreen_start(unsigned state);
static void port_o_loadscreen_start(unsigned state)
{
    struct port_loadscreen_fields f;
    (void)state;
    screen_fields(&f);
    port_loadscreen_start(&f);
}
static void port_o_loadscreen_fadeout(unsigned state);
static void port_o_loadscreen_fadeout(unsigned state)
{
    struct port_loadscreen_fields f;
    (void)state;
    screen_fields(&f);
    port_loadscreen_fadeout(&f);
}

const struct port_override port_loadwait_overrides[] = {
    { "start_cd_track_wait_with_thread_switch", (void *)port_o_wait_with_thread_switch,
      "starts the load and steps the disc and the game's load step with no wait for frames until it is done (setting fast_loads)", &port_settings.fast_loads },
    { "start_cd_track_and_wait_updating_control", (void *)port_o_wait_updating_control,
      "the same, with that routine's own screen flag set before each step and cleared after (setting fast_loads)", &port_settings.fast_loads },
    { "start_hud_stage_step_sequence", (void *)port_o_loadscreen_start,
      "the loading screen's first state does at once what its fade-in ends with, and blanks the text (setting fast_loads)", &port_settings.fast_loads },
    { "update_fade_step_counter_down", (void *)port_o_loadscreen_fadeout,
      "the loading screen's fade-out state does at once what the fade-out ends with (setting fast_loads)", &port_settings.fast_loads },
    { "fade_palette_bank", (void *)port_o_fade_palette_bank,
      "a black fade (mode 0 or 1) runs the routine's own steps back to back until it says done, so the fade takes one frame (setting fast_loads)", &port_settings.fast_loads },
    { NULL, NULL, NULL, NULL }
};
