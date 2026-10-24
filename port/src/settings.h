/* Project-authored typed settings API, shared by startup and the settings menu (port/settings-menu.md). */
#ifndef PORT_SETTINGS_H
#define PORT_SETTINGS_H
#include <stddef.h>
#include <stdio.h>
/* [display] screen: the window, exclusive fullscreen at a chosen size, or borderless fullscreen at the desktop's size. */
enum port_screen { PORT_SCREEN_WINDOW, PORT_SCREEN_FULLSCREEN, PORT_SCREEN_BORDERLESS };
/* [display] filter: how the finished picture is stretched to the window. */
enum port_filter { PORT_FILTER_NEAREST, PORT_FILTER_BILINEAR, PORT_FILTER_SHARP_BILINEAR };
#define PORT_WINDOW_DEFAULT_W 1280   /* PsyZ's own first window size */
#define PORT_WINDOW_DEFAULT_H 960
#define PORT_SIZE_MIN 320            /* a window or fullscreen size, each side, inclusive */
#define PORT_SIZE_MAX 16384
struct port_settings {
    /* [experimental], each 0 or 1 */
    int pixel_perfect_ui, arcade_video, center_ui, fast_loads, memory_card, arcade_intro, arcade_assets, settings_menu, widescreen;
    /* [display] */
    int screen;                     /* enum port_screen */
    int window_w, window_h;         /* the window's inner size in pixels, for PORT_SCREEN_WINDOW */
    int fullscreen_w, fullscreen_h; /* the mode for PORT_SCREEN_FULLSCREEN; 0, 0: the desktop's */
    int filter;                     /* enum port_filter */
};
extern struct port_settings port_settings;        /* what the program uses: the file, then the command line */
extern struct port_settings port_settings_file;   /* what the file gave (or the defaults); the menu's saves write this */
extern const char *port_settings_path;            /* the file that was read, and that the menu saves to */
void port_settings_defaults(struct port_settings *out);
int port_settings_read(FILE *file,struct port_settings *out,char *err,size_t size);
int port_settings_load(const char *path,struct port_settings *out,int missing_ok,char *err,size_t size);
int port_settings_save(const char *path,const struct port_settings *value,char *err,size_t size);
/* The file's form of *value, both sections, every key, as save writes it. 0, or -1 on a write error. */
int port_settings_write(FILE *file,const struct port_settings *value);
/* The names the file uses ("window", "sharp_bilinear", ...); NULL for a value out of range. */
const char *port_screen_name(int screen);
const char *port_filter_name(int filter);
#endif
