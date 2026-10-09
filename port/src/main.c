/* sfa2.exe DISC: map the PS1's memory, load the game's program from the disc
 * image, write the jumps, and call the game's main.
 *
 * Status: 0 main returned, 2 a refusal (one line says why), 3 a function
 * without C was reached, 4 a library function without a host routine was
 * reached, 5 an unknown function was reached; the statuses are 0, 2, 3, 4 and 5 only. */
#include "port.h"
#include "port_tables.h"

#include <stdlib.h>

static int refuse(const char *line)
{
    printf("refused: %s\n", line);
    fflush(stdout);
    return 2;
}

int main(int argc, char **argv)
{
    char err[PORT_ERR];
    struct port_disc disc;
    struct port_program prog;
    unsigned char *ram = (unsigned char *)(size_t)PORT_RAM_BASE;
    unsigned with_c, without_c, entry;

    if (argc != 2) {
        printf("usage: %s DISC (a .cue or the .bin itself)\n", argc > 0 ? argv[0] : "sfa2");
        fflush(stdout);
        return 2;
    }
    if (port_map(err, sizeof err) != 0) return refuse(err);
    printf("memory: RAM at 0x%08x (2 MB), scratchpad at 0x%08x\n", PORT_RAM_BASE, PORT_SCRATCH);
    fflush(stdout);

    if (port_disc_open(&disc, argv[1], err, sizeof err) != 0) return refuse(err);
    printf("disc: %s, %u-byte sectors\n", disc.path, PORT_SECTOR);
    fflush(stdout);

    if (port_program_load(&disc, ram, port_program_sha256, &prog, err, sizeof err) != 0) return refuse(err);
    port_disc_close(&disc);
    printf("program: %s at sector %u, %u bytes to 0x%08x, entry 0x%08x\n", prog.name, prog.sector, prog.t_size, prog.t_addr, prog.pc0);
    printf("identity: SHA-256 matches the build's baseline\n");
    fflush(stdout);

    if (port_jumps_write(ram, &with_c, &without_c, err, sizeof err) != 0) return refuse(err);
    printf("jumps: %u written for functions with C, %u for functions without\n", with_c, without_c);
    fflush(stdout);

    if (port_entry_scan(ram, prog.pc0, &entry) != 0) return refuse("start: no jal before a break among the first 64 instructions at the entry");
    if (!port_jump_known(entry)) {
        char line[PORT_ERR];
        snprintf(line, sizeof line, "the entry 0x%08x is not a function this build knows", entry);
        return refuse(line);
    }
    printf("start: 0x%08x\n", entry);
    fflush(stdout);

    ((void (*)(void))(size_t)entry)();
    port_stop_main_returned();
    return 0;
}
