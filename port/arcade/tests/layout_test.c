/* Driver of the disc layer's layout backend (port/src/disc.c; port/tools/test_arcade_layout.py): a made-up layout of a 40-sector disc is compiled in.
 *   [--layout] [--bad N] PACK list              one line a file: name sector size
 *   ... find NAME                               "sector size", exit 1 when the layout (or the pack's directory) has no such file
 *   ... raw SECTOR                              exit 1 when the sector is not served; else 24 header bytes in hex, then the sum of the 2048 data bytes
 *   ... read SECTOR SIZE                        the data bytes: exit 1 when not served; else the first 8 bytes in hex and the sum of all
 *   ... sub SECTOR                              the layout's subheader of the sector: exit 1 when it is in no stream
 * --layout presents the layout with the pack (exit 3 with the line when refused); --bad N changes the layout (1: a file overlaps the next,
 * 2: another length, 3: a file the pack has is missing, 4: a file of another size). */
#include "port.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct port_layout_file files_good[] = {{"a.dat", 2, 5000}, {"s.xa", 5, 16 * 2048}, {"gone.str", 21, 3 * 2048}};
static struct port_layout_file files_bad[4][3];
static const struct port_layout_stream streams[] = {{5, 16, {8, 9, 10, 11, 12, 3, 14, 15}}};
static const struct port_layout_fill fills[] = {{24, 3, 0x20}};
static struct port_layout layout = {40, files_good, 3, streams, 1, fills, 1};

static struct port_disc d;
static void closer(void) { port_disc_close(&d); }

int main(int argc, char **argv)
{
    char err[256];
    int use = 0, a = 1, bad = 0;
    unsigned i;
    memset(err, 0, sizeof err);
    while (a < argc && argv[a][0] == '-') {
        if (!strcmp(argv[a], "--layout")) use = 1;
        else if (!strcmp(argv[a], "--bad") && a + 1 < argc) bad = atoi(argv[++a]);
        a++;
    }
    if (argc - a < 2) return 2;
    if (port_disc_open_pack(&d, argv[a], err, sizeof err) != 0) { printf("%s\n", err); return 2; }
    atexit(closer);
    memcpy(files_bad[0], files_good, sizeof files_good);
    files_bad[0][0].size = 5 * 2048;   /* a.dat now ends at sector 6 and overlaps the stream */
    if (bad == 1) { layout.files = files_bad[0]; }
    if (bad == 2) layout.sectors = 41;
    if (bad == 3) { memcpy(files_bad[2], files_good, sizeof files_good); files_bad[2][0] = files_good[1]; files_bad[2][1] = files_good[2]; layout.files = files_bad[2]; layout.file_count = 2; }
    if (bad == 4) { memcpy(files_bad[3], files_good, sizeof files_good); files_bad[3][0].size = 4000; layout.files = files_bad[3]; }
    if (use && port_disc_use_layout(&d, &layout, err, sizeof err) != 0) { printf("%s\n", err); return 3; }
    if (!strcmp(argv[a + 1], "list")) {
        struct port_disc_file out[16];
        unsigned n;
        if (port_disc_list(&d, out, 16, &n, err, sizeof err) != 0) { printf("%s\n", err); return 1; }
        for (i = 0; i < n; i++) printf("%s %u %u\n", out[i].name, out[i].sector, out[i].size);
        return 0;
    }
    if (!strcmp(argv[a + 1], "find") && argc - a >= 3) {
        unsigned sector, size;
        if (port_disc_find(&d, argv[a + 2], &sector, &size, err, sizeof err) != 0) { printf("%s\n", err); return 1; }
        printf("%u %u\n", sector, size);
        return 0;
    }
    if (!strcmp(argv[a + 1], "raw") && argc - a >= 3) {
        unsigned char raw[PORT_SECTOR];
        unsigned sum = 0;
        if (port_disc_raw(&d, (unsigned)atoi(argv[a + 2]), raw) != 0) return 1;
        for (i = 0; i < 24; i++) printf("%02x", raw[i]);
        for (i = 24; i < 24 + 2048; i++) sum += raw[i];
        printf(" %u\n", sum);
        return 0;
    }
    if (!strcmp(argv[a + 1], "read") && argc - a >= 4) {
        unsigned size = (unsigned)atoi(argv[a + 3]), sum = 0;
        unsigned char *b = malloc(size);
        if (!b || port_disc_read(&d, (unsigned)atoi(argv[a + 2]), size, b, err, sizeof err) != 0) return 1;
        for (i = 0; i < 8 && i < size; i++) printf("%02x", b[i]);
        for (i = 0; i < size; i++) sum += b[i];
        printf(" %u\n", sum);
        free(b);
        return 0;
    }
    if (!strcmp(argv[a + 1], "sub") && argc - a >= 3) {
        unsigned char sub[8];
        if (!port_layout_subheader(&layout, (unsigned)atoi(argv[a + 2]), sub)) return 1;
        for (i = 0; i < 8; i++) printf("%02x", sub[i]);
        printf("\n");
        return 0;
    }
    return 2;
}
