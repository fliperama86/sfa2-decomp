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

/* func_80119694 (resident_nonmatching/func_80119694.c): compacts an ordering table. Each word of the table
 * holds the 24-bit form of the address of the word before it (a link), or something else; the function walks
 * down from the address it is given (at most 29 words), and for a run of links stores in the first word of
 * the run the 24-bit form of the address where the run ends. The original masks its argument with 0xffffff and
 * reads RAM through the PS1's mirror at address 0, which a Windows process cannot map. This one reads the same
 * words at their real addresses (0x80000000 or'ed into the masked argument), compares each word with the 24-bit
 * form of its neighbour's address, and stores the 24-bit form, as the original does.
 * Not complete as a guard: a read of the mirror that lands on one of the system's own tables does not fault and
 * cannot be seen. */
#define F24(p) ((unsigned)(size_t)(p) & 0xffffffu)
void port_o_func_80119694(void *a);
void port_o_func_80119694(void *a)
{
    unsigned *p = (unsigned *)(size_t)(0x80000000u | ((unsigned)(size_t)a & 0xffffffu));
    unsigned *last;
    int n = 29;

    for (;;) {
        if (*p != F24(p - 1)) {
            if (--n == 0) return;
            p--;
            continue;
        }
        if (--n == 0) return;
        last = p;
        p--;
        while (*p == F24(p - 1)) {
            if (--n == 0) return;
            p--;
        }
        *last = F24(p);
        if (--n == 0) return;
        p--;
    }
}

/* FlushCache: there is no instruction cache to flush on a PC. */
void port_h_FlushCache(void);
void port_h_FlushCache(void)
{
}

const struct port_override port_game_overrides[] = {
    { "func_80119030", (void *)port_o_func_80119030, "same initialisation of the three task slots as its C, without the stores into the BIOS's thread table (address 0x110 is not mapped)" },
    { "func_80119694", (void *)port_o_func_80119694, "compacts an ordering table through the PS1's RAM mirror at address 0, which a Windows process cannot map: the same walk on the real addresses (a read of the mirror that lands on one of the system's tables does not fault and cannot be seen, so the crash guard is not complete)" },
    { NULL, NULL, NULL }
};

const struct port_library port_system_library[] = {
    { "FlushCache", (void *)port_h_FlushCache, "no instruction cache to flush on a PC" },
    { NULL, NULL, NULL }
};
