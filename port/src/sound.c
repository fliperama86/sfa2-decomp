/* Sound: accepted, nothing sounds (this piece of the port has no sound yet).
 *
 * Every routine of the table takes the call and returns what the game takes as
 * success; the note on each entry says so, for --list-library. Where the game
 * compares a result, it is given by the game's own use: SsVabTransBodyPartly
 * returns the vab id it was given (the game compares the two: d25684_r2.c,
 * s14f9f0_r4.c), SsVabTransCompleted returns 1, transfer complete (the game calls it with
 * 1 and waits on it, PSY-Q's SS_WAIT_COMPLETED). The rest return 0. */
#include "port.h"

/* With PORT_SOUND_LIBRARY_C (hostbuild.py --sound-library) the library's functions have C in the program, and a host
 * routine for a function that has C is refused when the program starts: the table is empty then, and the four
 * routines that only the table uses are left out. */
#ifndef PORT_SOUND_LIBRARY_C

/* No sound: the call is accepted and nothing sounds; 0. */
int port_h_sound_ok(void);
int port_h_sound_ok(void)
{
    return 0;
}

/* SsVabTransBodyPartly(addr, bufsize, vabid): PSY-Q's documentation and the game's use (it compares
 * the result with the vab id it passed): returns the vab id. */
int port_h_SsVabTransBodyPartly(void *addr, unsigned size, int vabid);
int port_h_SsVabTransBodyPartly(void *addr, unsigned size, int vabid)
{
    (void)addr;
    (void)size;
    return vabid;
}

/* SsVabOpenHeadSticky(addr, vabid, sbaddr): PSY-Q's documentation: returns the vab id (0 where the caller asked for any, -1). */
int port_h_SsVabOpenHeadSticky(void *addr, int vabid, unsigned sbaddr);
int port_h_SsVabOpenHeadSticky(void *addr, int vabid, unsigned sbaddr)
{
    (void)addr;
    (void)sbaddr;
    return vabid == -1 ? 0 : vabid;
}

/* SsVabTransCompleted(mode): 1, the transfer is complete (SS_IMEDIATE / SS_WAIT_COMPLETED); nothing is ever sent. */
int port_h_SsVabTransCompleted(int mode);
int port_h_SsVabTransCompleted(int mode)
{
    (void)mode;
    return 1;
}

#define NOTE "no sound yet: the call is accepted and nothing sounds"
#define OK(name) { name, (void *)port_h_sound_ok, NOTE }

const struct port_library port_sound_library[] = {
    { "SsVabTransBodyPartly", (void *)port_h_SsVabTransBodyPartly, NOTE ": returns the vab id it was given" },
    { "SsVabOpenHeadSticky", (void *)port_h_SsVabOpenHeadSticky, NOTE ": returns the vab id" },
    { "SsVabTransCompleted", (void *)port_h_SsVabTransCompleted, NOTE ": returns 1, complete" },
    OK("SpuVmSetSeqVol"), OK("SsUtKeyOnV"), OK("SsUtKeyOffV"), OK("SsUtSetVVol"), OK("SsUtAllKeyOff"),
    OK("SsSeqSetVol"), OK("SsInit"), OK("SsSetTableSize"), OK("SsSetTickMode"), OK("SsStart2"), OK("SsQuit"),
    OK("SsSetMono"), OK("SsSetStereo"), OK("SsSetMVol"), OK("SsSetSerialAttr"), OK("SpuSetCommonAttr"),
    OK("SpuSetTransferMode"), OK("_SpuInit"), OK("_spu_write"), OK("SsSepPlay"), OK("SsSepStop"),
    OK("SsSeqClose"), OK("SsSepClose"), OK("SsVabClose"), OK("SsSeqOpen"), OK("func_80166144"),
    { NULL, NULL, NULL }
};

#else

const struct port_library port_sound_library[] = {
    { NULL, NULL, NULL }
};

#endif
