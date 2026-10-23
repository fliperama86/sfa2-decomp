/* Overrides: host routines that run INSTEAD of the C of a game function, for what
 * cannot run on a PC as written. Each has a note (shown by --list-library). */
#include "port.h"
#include "cd.h"
#include "programs.h"

/* init_snd_voices (resident/sound/init_snd_voices.c): initialises the game's three task slots (table at 0x801fc200, 0x80 bytes each:
 * a u16 state at offset 0, the u32 stack pointer for OpenTh at offset 8) and, as its C does through the
 * pointer the BIOS keeps at address 0x110, writes 0x40000404 into the BIOS's thread control blocks (stride
 * 0xc0 from that pointer + 0x94). That second part reads address 0x110, which is not mapped here, and
 * there is no BIOS thread table: it is left out. The first part is the same stores as the C. */
static void port_o_func_80119030(void);
static void port_o_func_80119030(void)
{
    int i;
    for (i = 0; i < 3; i++) {
        unsigned char *v = port_gs((const void *)(size_t)(0x801fc200u + (unsigned)i * 0x80u), 12);   /* the stores below reach offsets 0 to 1 and 8 to 11 */
        *(unsigned short *)v = 0;
        *(unsigned *)(v + 8) = 0x801fec00u + (unsigned)i * 0x800u;
    }
}

/* Exec(header, argc, argv): the game runs stand-alone programs of the disc with it (PAC/LOGO.EXE at start, then from the
 * attract thread PAC/Z2O.EXE and, from the menu code, PAC/BF3.EXE: each a program of its own with its own entry,
 * for which no C exists yet). The file the game asked for is found from the disc sector that the disc layer wrote
 * into the first word of the header (0x801e0000). A program whose C is built in (a row of port_programs, programs.c) is
 * verified against the disc and run, with or without --skip-programs, and Exec returns 1. Any other: the program ends with
 * a line that names the file and the sector (status 13); with --skip-programs it prints a line at each skip and returns 1 at
 * once instead (the option skips the programs that have no C). */
int port_h_Exec(void *header, int argc, char **argv);
int port_h_Exec(void *header, int argc, char **argv)
{
    static struct port_disc_file files[512];
    unsigned count = 0, i;
    char err[PORT_ERR];
    int sector = port_cd_source_at((unsigned)(size_t)header);
    const char *name = "?";
    unsigned file_sector = 0, file_size = 0;

    (void)argc;
    (void)argv;
    if (sector >= 0 && port_disc_handle && port_disc_list(port_disc_handle, files, 512, &count, err, sizeof err) == 0)
        for (i = 0; i < count; i++)
            if ((unsigned)sector >= files[i].sector && (unsigned)sector < files[i].sector + (files[i].size + 2047) / 2048) {
                name = files[i].name;
                file_sector = files[i].sector;
                file_size = files[i].size;
            }
    if (port_program_exec(name, file_sector, file_size, header)) return 1;
    if (!port_skip_programs) port_halt(PORT_EXIT_PROGRAM, "no C yet for the program %s (disc sector %d)", name, sector);
    printf("skipped: Exec of %s (disc sector %d): a program of its own for which no C exists yet; it returns at once\n", name, sector);
    fflush(stdout);
    return 1;
}

/* FlushCache: there is no instruction cache to flush on a PC. */
void port_h_FlushCache(void);
void port_h_FlushCache(void)
{
}

const struct port_override port_game_overrides[] = {
    { "init_snd_voices", (void *)port_o_func_80119030, "same initialisation of the three task slots as its C, without the stores into the BIOS's thread table (address 0x110 is not mapped)", NULL },
    { NULL, NULL, NULL, NULL }
};

const struct port_library port_system_library[] = {
    { "Exec", (void *)port_h_Exec, "a stand-alone program of the disc whose C is built in is verified and run; one without C ends the run, or with --skip-programs returns 1 at once" },
    { "FlushCache", (void *)port_h_FlushCache, "no instruction cache to flush on a PC" },
    { NULL, NULL, NULL }
};
