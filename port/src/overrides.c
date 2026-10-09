/* Overrides: host routines that run INSTEAD of the C of a game function, for what
 * cannot run on a PC as written. Each has a note (shown by --list-library). */
#include "port.h"

/* func_80119030 (v22_r2.c): initialises the game's three task slots (table at 0x801fc200, 0x80 bytes each:
 * a u16 state at offset 0, the u32 stack pointer for OpenTh at offset 8) and, as its C does through the
 * pointer the BIOS keeps at address 0x110, writes 0x40000404 into the BIOS's thread control blocks (stride
 * 0xc0 from that pointer + 0x94). That second part reads address 0x110, which is not mapped here, and
 * there is no BIOS thread table: it is left out. The first part is the same stores as the C. */
static void port_o_func_80119030(void);
static void port_o_func_80119030(void)
{
    int i;
    for (i = 0; i < 3; i++) {
        unsigned char *v = (unsigned char *)(size_t)(0x801fc200u + (unsigned)i * 0x80u);
        *(unsigned short *)v = 0;
        *(unsigned *)(v + 8) = 0x801fec00u + (unsigned)i * 0x800u;
    }
}

/* FlushCache: there is no instruction cache to flush on a PC. */
void port_h_FlushCache(void);
void port_h_FlushCache(void)
{
}

const struct port_override port_game_overrides[] = {
    { "func_80119030", (void *)port_o_func_80119030, "same initialisation of the three task slots as its C, without the stores into the BIOS's thread table (address 0x110 is not mapped)" },
    { NULL, NULL, NULL }
};

const struct port_library port_system_library[] = {
    { "FlushCache", (void *)port_h_FlushCache, "no instruction cache to flush on a PC" },
    { NULL, NULL, NULL }
};
