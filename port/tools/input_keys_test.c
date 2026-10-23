/* The settings menu's host keys in the input script (input.c, port/settings-menu.md): a down line or a repeat's press of
 * f1, escape, f4 or guide is a press for port_input_take_keys at its frame, and no pad button changes. Built without PsyZ. */
#include "port.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static unsigned word(const unsigned char *f) { return ~(unsigned)(f[3] | f[2] << 8) & 0xffffu; }

static unsigned at(unsigned frame, unsigned *pad)
{
    unsigned char f[4] = { 0x00, 0x41, 0xff, 0xff };
    port_input_apply(f, frame);
    *pad = word(f);
    return port_input_take_keys();
}

int main(int argc, char **argv)
{
    char err[PORT_INPUT_ERR];
    unsigned pad, frame;
    FILE *file;
    assert(argc == 2);
    file = fopen(argv[1], "w");
    assert(file);
    /* f1 at frame 60, its up line at 61; cross held 62..119; f4 and escape together at frame 120; guide repeated every 0.5 s from 3 s,
       until the line at 4.2 s (frames 180, 210, 240) */
    fputs("1 f1 down\n1.0167 f1 up\n1.0333 cross down\n2 cross up\n2 f4 down\n2 escape down\n2.5 f4 up\n2.5 escape up\nrepeat 3 0.5 guide\n4.2 start down\n4.3 start up\n", file);
    fclose(file);
    assert(port_input_load(argv[1], err, sizeof err) == 0);
    for (frame = 1; frame <= 300; frame++) {
        unsigned keys = at(frame, &pad);
        unsigned want = frame == 60 ? PORT_INPUT_KEY_MENU
                      : frame == 120 ? (PORT_INPUT_KEY_FULLSCREEN | PORT_INPUT_KEY_ESCAPE)
                      : (frame == 180 || frame == 210 || frame == 240) ? PORT_INPUT_KEY_GUIDE : 0;
        unsigned want_pad = (frame >= 62 && frame < 120 ? 1u << 6 : 0) | (frame >= 252 && frame < 258 ? 1u << 11 : 0);
        if (keys != want || pad != want_pad) {
            printf("frame %u: keys %x (wanted %x), pad %04x (wanted %04x)\n", frame, keys, want, pad, want_pad);
            return 1;
        }
    }
    /* keys of frames passed in one step are all kept until taken */
    assert(port_input_load(argv[1], err, sizeof err) == 0);
    {
        unsigned char f[4] = { 0x00, 0x41, 0xff, 0xff };
        port_input_apply(f, 130);
        assert(port_input_take_keys() == (PORT_INPUT_KEY_MENU | PORT_INPUT_KEY_FULLSCREEN | PORT_INPUT_KEY_ESCAPE));
        assert(port_input_take_keys() == 0);
    }
    /* a host key name in capitals is not one */
    file = fopen(argv[1], "w");
    fputs("1 F1 down\n", file);
    fclose(file);
    assert(port_input_load(argv[1], err, sizeof err) != 0 && strstr(err, "`F1` is not a button"));
    puts("input keys ok");
    return 0;
}
