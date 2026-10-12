/* The stand-alone programs of the disc whose C is built into the PC program: see programs.h. */
#include "port.h"
#include "port_tables.h"
#include "programs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAM_END (PORT_RAM_BASE + (unsigned long long)PORT_RAM_SIZE)

static int lower(int c)
{
    return c >= 'A' && c <= 'Z' ? c + 32 : c;
}

/* Does the base name of `disc_name` (after the last / or \, before a `;` suffix) equal `file` without regard to case? */
static int same_file(const char *disc_name, const char *file)
{
    const char *base = disc_name, *p;
    size_t n;
    for (p = disc_name; *p; p++)
        if (*p == '/' || *p == '\\') base = p + 1;
    for (n = 0; base[n] && base[n] != ';'; n++) {}
    if (strlen(file) != n) return 0;
    for (p = file; *p; p++, base++)
        if (lower((unsigned char)*p) != lower((unsigned char)*base)) return 0;
    return 1;
}

static void hex(const unsigned char h[32], char out[65])
{
    int i;
    for (i = 0; i < 32; i++) sprintf(out + 2 * i, "%02x", h[i]);
}

static unsigned word(const unsigned char *p)
{
    return (unsigned)p[0] | (unsigned)p[1] << 8 | (unsigned)p[2] << 16 | (unsigned)p[3] << 24;
}

int port_program_exec(const char *disc_name, unsigned sector, unsigned size, void *header)
{
    static const char *const field[4] = { "pc0", "gp0", "t_addr", "t_size" };
    const struct port_builtin_program *row = 0;
    unsigned char *file, got[32];
    char err[PORT_ERR], a[65], b[65];
    unsigned i, t_addr, t_size;
    unsigned long long h = (unsigned long long)(size_t)header, at;
    const unsigned char *ram;

    for (i = 0; i < port_program_count; i++)
        if (port_programs[i].file && same_file(disc_name, port_programs[i].file)) {
            row = &port_programs[i];
            break;
        }
    if (!row) return 0;

    /* the file */
    if (!port_disc_handle) port_halt(PORT_EXIT_PROGRAM, "program %s (%s): no disc is open", row->name, disc_name);
    file = (unsigned char *)malloc(size ? size : 1);
    if (!file) port_halt(PORT_EXIT_PROGRAM, "program %s (%s): no memory for the file's %u bytes", row->name, disc_name, size);
    if (port_disc_read(port_disc_handle, sector, size, file, err, sizeof err) != 0)
        port_halt(PORT_EXIT_PROGRAM, "program %s (%s): the file cannot be read: %s", row->name, disc_name, err);
    port_sha256(file, size, got);
    if (memcmp(got, row->sha256, 32) != 0) {
        hex(got, a);
        hex(row->sha256, b);
        port_halt(PORT_EXIT_PROGRAM, "program %s: the file %s on the disc has SHA-256 %s, the C was written from the file with %s", row->name, disc_name, a, b);
    }

    /* the header */
    if (size < 0x20) port_halt(PORT_EXIT_PROGRAM, "program %s (%s): the file has %u bytes, it has no header", row->name, disc_name, size);
    if (h < PORT_RAM_BASE || h + 16 > RAM_END)
        port_halt(PORT_EXIT_PROGRAM, "program %s: the header at 0x%llx, 16 bytes, is not inside the game's RAM", row->name, h);
    ram = port_gs((const void *)(size_t)h, 16);
    for (i = 0; i < 4; i++)
        if (memcmp(ram + 4 * i, file + 0x10 + 4 * i, 4) != 0)
            port_halt(PORT_EXIT_PROGRAM, "program %s: the header in RAM differs from the file's at %s: RAM 0x%08x, file 0x%08x", row->name, field[i],
                      word(ram + 4 * i), word(file + 0x10 + 4 * i));

    /* the body */
    t_addr = word(file + 0x18);
    t_size = word(file + 0x1c);
    if (t_size && 0x800ull + t_size > size)
        port_halt(PORT_EXIT_PROGRAM, "program %s: the header says t_size 0x%x, the file's %u bytes hold only %u after the first 0x800", row->name, t_size, size, size > 0x800 ? size - 0x800 : 0);
    if (t_addr < PORT_RAM_BASE || (unsigned long long)t_addr + t_size > RAM_END)
        port_halt(PORT_EXIT_PROGRAM, "program %s: the body at 0x%08x, 0x%x bytes, is not inside the game's RAM", row->name, t_addr, t_size);
    ram = port_gs((const void *)(size_t)t_addr, t_size);
    for (at = 0; at < t_size; at++)
        if (ram[at] != file[0x800 + at])
            port_halt(PORT_EXIT_PROGRAM, "program %s: the body in RAM differs from the file's at 0x%08x: RAM 0x%02x, file 0x%02x", row->name,
                      (unsigned)(t_addr + at), ram[at], file[0x800 + at]);
    free(file);

    port_callbacks_program_start();
    port_trace_line("program %s: start", row->name);
    if (row->setup) row->setup();
    ((void (*)(void))row->main)();
    if (row->teardown) row->teardown();
    port_trace_line("program %s: returned", row->name);
    port_press_report(row->name);   /* one line: what the program's movie made of the decoder */
    return 1;
}
