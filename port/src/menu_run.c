/* The host side of the settings menu (setting settings_menu; port/settings-menu.md): the keys, the pause, the picture and
 * the saves. menu.c has the menu itself. Runs on the game's thread at the end of a vblank (kernel.c), when the vblank's
 * work is done: the game is between two of its own instructions or waiting in a library routine, and none of its code
 * runs until the menu returns, so the game is paused, its sound with it. */
#include "port.h"
#include "gpu.h"
#include "menu.h"
#include "settings.h"

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

#define MENU_FRAME_US  16667ull   /* the menu's own frame, 1/60 s */
#define RELEASE_FRAMES 30         /* at most half a second for the buttons to be let go after closing */

static int requested;                              /* a key asked for the menu and it could not open yet */
static int fullscreen_kind = PORT_SCREEN_FULLSCREEN;   /* what F4 goes to from the window: the last of the two used */
static uint16_t *vram;                             /* the copy of the video memory while the menu is open */
static uint16_t picture[PORT_MENU_W * PORT_MENU_H];   /* the menu's display as it draws it */

/* Waits until the clock reaches `t`. A signal (Linux's vblank signal of interrupt.c) may end a sleep early: the loop
 * sleeps again. */
static void wait_until(unsigned long long t)
{
    for (;;) {
        unsigned long long now = port_now_us(), left;
        if (now >= t) return;
        left = t - now;
        if (left > 2000) left = 2000;
#ifdef _WIN32
        Sleep(left >= 1000 ? 1 : 0);
#else
        {
            struct timespec s;
            s.tv_sec = 0;
            s.tv_nsec = (long)(left * 1000ull);
            nanosleep(&s, NULL);
        }
#endif
    }
}

/* Port 1 now, as the pad word (input.c's order, 1 = pressed), with the input script's buttons of this frame; 0 when it
 * reports no controller. Polling also pumps the window's events, which is where PsyZ records the host keys. The script's
 * clock counts the menu's frames (port_alive), so a script can drive the menu. */
static unsigned pad_word(void)
{
    unsigned char f[PORT_PAD_FRAME];
    port_pad_host_read(f, (int)sizeof f);
    if (f[0] != 0 || f[1] == 0xff) return 0;
    port_input_apply(f, port_alive());
    return ~(unsigned)(f[3] | f[2] << 8) & 0xffffu;
}

/* The host keys pressed since the last call: PsyZ's (the keyboard, a gamepad's guide button) and the input script's. */
static unsigned take_keys(void)
{
    return port_gpu_take_keys() | port_input_take_keys();
}

static void host_info(struct port_menu_host *host)
{
    memset(host, 0, sizeof *host);
    if (port_gpu_desktop_size(&host->desktop_w, &host->desktop_h)) host->desktop_w = host->desktop_h = 0;
    host->modes = port_gpu_fullscreen_modes(host->mode_wh, PORT_MENU_MODES_MAX);
}

/* Applies what changed of the [display] settings from `before` to *s; a screen that the system refused leaves the one in effect,
 * and *s says so. */
static void apply(const struct port_settings *before, struct port_settings *s)
{
    int w = 0, h = 0;
    if (s->screen != before->screen || s->window_w != before->window_w || s->window_h != before->window_h ||
        s->fullscreen_w != before->fullscreen_w || s->fullscreen_h != before->fullscreen_h) {
        if (s->screen == PORT_SCREEN_WINDOW) {
            w = s->window_w;
            h = s->window_h;
        } else if (s->screen == PORT_SCREEN_FULLSCREEN) {
            w = s->fullscreen_w;
            h = s->fullscreen_h;
        }
        if (port_gpu_set_screen(s->screen, w, h)) {
            printf("settings: the screen could not be changed; it stays %s\n", port_screen_name(port_gpu_screen()));
            s->screen = port_gpu_screen();
        }
        if (s->screen != PORT_SCREEN_WINDOW) fullscreen_kind = s->screen;
    }
    if (s->filter != before->filter && port_gpu_set_filter(s->filter)) {
        printf("settings: this build cannot use the filter %s\n", port_filter_name(s->filter));
        s->filter = before->filter;
    }
    fflush(stdout);
}

/* Copies the [display] keys that changed from `before` to `after` into the file's settings and saves those: a command
 * line override of another key never reaches the file. */
static void save(const struct port_settings *before, const struct port_settings *after)
{
    char err[256];
    struct port_settings *f = &port_settings_file;
    if (after->screen != before->screen) f->screen = after->screen;
    if (after->window_w != before->window_w || after->window_h != before->window_h) {
        f->window_w = after->window_w;
        f->window_h = after->window_h;
    }
    if (after->fullscreen_w != before->fullscreen_w || after->fullscreen_h != before->fullscreen_h) {
        f->fullscreen_w = after->fullscreen_w;
        f->fullscreen_h = after->fullscreen_h;
    }
    if (after->filter != before->filter) f->filter = after->filter;
    if (port_settings_save(port_settings_path, f, err, sizeof err)) {
        printf("settings: not saved: %s\n", err);
        fflush(stdout);
    }
}

/* F4: the window, or the fullscreen kind used last. */
static void toggle_fullscreen(void)
{
    struct port_settings before = port_settings;
    port_settings.screen = port_settings.screen == PORT_SCREEN_WINDOW ? fullscreen_kind : PORT_SCREEN_WINDOW;
    apply(&before, &port_settings);
    save(&before, &port_settings);
}

/* The rectangle (x, y, PORT_MENU_W, PORT_MENU_H) of the copy into the picture buffer: what the video memory held there. */
static void copy_rect(int x, int y)
{
    int row;
    for (row = 0; row < PORT_MENU_H; row++)
        memcpy(picture + (size_t)row * PORT_MENU_W, vram + (size_t)(y + row) * PORT_MENU_VRAM_W + (size_t)x, PORT_MENU_W * sizeof *picture);
}

static void run(void)
{
    struct port_menu m;
    struct port_menu_host host;
    unsigned long long start, next;
    unsigned k;
    int x, y, gx, gy, gw, gh;

    if (!vram) vram = malloc((size_t)PORT_MENU_VRAM_W * PORT_MENU_VRAM_H * sizeof *vram);
    if (!vram || port_gpu_menu_snapshot(vram)) {
        printf("settings menu: cannot open (no memory, or no picture to read)\n");
        fflush(stdout);
        return;
    }
    port_gpu_menu_begin(&x, &y, &gx, &gy, &gw, &gh);
    take_keys();   /* a key pressed before the menu opened does not close it */
    host_info(&host);
    port_menu_open(&m, pad_word());
    printf("settings menu: open\n");
    fflush(stdout);
    start = next = port_now_us();
    for (;;) {
        struct port_settings before;
        unsigned keys, pad;
        int act;
        port_menu_draw(&m, &port_settings, vram, gx, gy, gw, gh, picture);
        port_gpu_menu_show(picture, x, y, PORT_MENU_W, PORT_MENU_H);
        port_gpu_present();
        port_clock_menu_frame();
        port_debug_tick(port_alive());   /* --dump-every pictures go on while the menu is open */
        next += MENU_FRAME_US;
        if (port_now_us() > next + 4 * MENU_FRAME_US) next = port_now_us();   /* far behind (a slow screen change): no catching up */
        wait_until(next);
        pad = pad_word();
        keys = take_keys();
        if (keys & (PORT_GPU_KEY_MENU | PORT_GPU_KEY_ESCAPE | PORT_GPU_KEY_GUIDE)) break;
        if (keys & PORT_GPU_KEY_FULLSCREEN) {
            toggle_fullscreen();
            host_info(&host);
            continue;
        }
        before = port_settings;
        act = port_menu_step(&m, pad, &host, &port_settings);
        if (act == PORT_MENU_CHANGED) {
            apply(&before, &port_settings);
            if (port_settings.screen != before.screen) host_info(&host);
            save(&before, &port_settings);
        } else if (act == PORT_MENU_CLOSE) {
            break;
        } else if (act == PORT_MENU_QUIT_GAME) {
            printf("stop: quit from the settings menu\n");
            fflush(stdout);
            exit(0);
        }
    }
    /* The video memory under the menu and the game's display as they were, and no button that the menu saw pressed reaches the game. */
    copy_rect(x, y);
    port_gpu_menu_show(picture, x, y, PORT_MENU_W, PORT_MENU_H);
    port_gpu_menu_end();
    port_gpu_present();
    for (k = 0; k < RELEASE_FRAMES && pad_word(); k++) {
        next += MENU_FRAME_US;
        wait_until(next);
        port_gpu_present();
        port_clock_menu_frame();
    }
    take_keys();   /* a key pressed while letting go does not open the menu again */
    port_clock_paused(port_now_us() - start);
    printf("settings menu: closed\n");
    fflush(stdout);
}

void port_menu_vblank(void)
{
    static int started;
    unsigned keys;
    if (!port_settings.settings_menu) return;
    if (!started) {
        started = 1;   /* F4's first fullscreen kind is the one the settings name, if any */
        if (port_settings.screen != PORT_SCREEN_WINDOW) fullscreen_kind = port_settings.screen;
    }
    keys = take_keys();
    if (keys & PORT_GPU_KEY_FULLSCREEN) toggle_fullscreen();
    if (keys & (PORT_GPU_KEY_MENU | PORT_GPU_KEY_ESCAPE | PORT_GPU_KEY_GUIDE)) requested = !requested;
    if (!requested || port_clock_hurrying() || !port_gpu_menu_allowed()) return;
    requested = 0;
    run();
}

/* The mode select's GAME INFORMATION (the attract module slot0f of DEMO.PAC, at 0x800df000) starts the stand-alone program BF3.EXE, a
 * movie, which the port does not run. With the menu on, the item is PC OPTIONS and opens the menu. The game's own flow stays: after the
 * program the game starts its session again, at the title. Its record in the module's data: the text at 0x800df094 (20 bytes up to the
 * next string, the mode select's strings end with "@@"), the record at 0x800e84e0 (draw_text_items' item: x at +4, the text's address at
 * +12). The game centres the item in 368 pixels: x 56 for 16 characters of 16 pixels, 104 for 10. Read from the disc's DEMO.PAC on
 * 2026-10-23; the bytes are checked before they are changed, so another module or another build of its data is left as it is. */
#define INFO_TEXT 0x800df094u
#define INFO_ITEM 0x800e84e0u
static const char info_old[] = "GAME INFORMATION@@";
static const char info_new[] = "PC OPTIONS@@";

void port_menu_module_placed(const char *name)
{
    static int said;
    unsigned char *text, *item;
    int ok;
    if (!port_settings.settings_menu || strcmp(name, "slot0f") != 0) return;
    text = port_gs((const void *)(uintptr_t)INFO_TEXT, sizeof info_old - 1);
    item = port_gs((const void *)(uintptr_t)INFO_ITEM, 16);
    ok = text && item && memcmp(text, info_old, sizeof info_old - 1) == 0 && (item[4] | item[5] << 8) == 56 &&
         (item[12] | item[13] << 8 | (unsigned)item[14] << 16 | (unsigned)item[15] << 24) == INFO_TEXT;
    if (ok) {
        memcpy(text, info_new, sizeof info_new - 1);
        item[4] = 104;
        item[5] = 0;
    }
    if (!said) {
        said = 1;
        printf(ok ? "settings menu: the mode select's GAME INFORMATION is PC OPTIONS\n"
                  : "settings menu: the mode select's GAME INFORMATION is not where it was read from; it stays as it is\n");
        fflush(stdout);
    }
}

int port_menu_program(const char *name)
{
    if (!port_settings.settings_menu || strcmp(name, "bf3.exe") != 0 || !port_gpu_menu_allowed()) return 0;
    printf("settings menu: in place of %s (PC OPTIONS)\n", name);
    fflush(stdout);
    run();
    return 1;
}
